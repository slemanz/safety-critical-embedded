/** 
 * @file gpio_demo.c
 * @brief GPIO driver demonstration interface
 * 
 * This file provides a demonstration interface for the safe GPIO driver
 * allowing interaction via UART commnads
 * 
 * @version 1.0
 */
#include "gpio_demo.h"
#include "safe_gpio.h"
#include "uart_debug.h"
#include <string.h>
#include <stdlib.h>

/* Demo pin configuration tracking*/
#define MAX_DEMO_PINS               (8U)

/* Demo configuration and state tracking */
static uint8_t demo_initialized = 0U;

/* Command processing state */
typedef enum{
    DEMO_STATE_WAITING_FOR_COMMAND,
    DEMO_STATE_PROCESSING_COMMAND
}Demo_State_t;

static Demo_State_t demo_state = DEMO_STATE_WAITING_FOR_COMMAND;

typedef struct{
    uint8_t configured;
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    GPIO_Mode_t mode;
}Demo_PinConfig_t;

static Demo_PinConfig_t configured_pins[MAX_DEMO_PINS];

/**
 * @brief Find a free slot for pin configuration
 * 
 * @return int8_t Index of the free slot, or -1 if no free slot
 */
static int8_t FindFreePinSlot(void){
    uint8_t i;

    for(i = 0U; i < MAX_DEMO_PINS; i++){
        if(configured_pins[i].configured == 0U){
            return (int8_t)i;
        }
    }

    return -1;
}

/**
 * @brief Find a configured pin by port and pin number
 * 
 * @param[in] port GPIO port
 * @param[in] pin GPIO pin
 * @return int8_t Index of the configured pin, or -1 if not found
 */
static int8_t FindConfiguredPin(GPIO_Port_t port, GPIO_Pin_t pin)
{
    uint8_t i;

    for(i = 0U; i < MAX_DEMO_PINS; i++){
        if((configured_pins[i].configured != 0U) &&
           (configured_pins[i].port == port) &&
           (configured_pins[i].pin == pin)){
            return (int8_t)i;
        }
    }

    return -1;
}

/**
 * @brief Convert a port string to a GPIO_Port_t
 * 
 * @param[in] port_str Port string (e.g. "A", "B", "C")
 * @param[out] port Pointer to store the port
 * @return Status_t Operation status
 */
static Status_t ParsePort(const char* port_str, GPIO_Port_t* port)
{
    if((port_str == NULL) || (port == NULL)){
        return STATUS_ERROR_NULL;
    }

    if(strlen(port_str) != 1U){
        return STATUS_ERROR_PARAM;
    }

    switch(port_str[0]){
        case 'A':
        case 'a':
            *port = GPIO_PORT_A;
            break;
        case 'B':
        case 'b':
            *port = GPIO_PORT_B;
            break;
        case 'C':
        case 'c':
            *port = GPIO_PORT_C;
            break;
        case 'D':
        case 'd':
            *port = GPIO_PORT_D;
            break;
        case 'E':
        case 'e':
            *port = GPIO_PORT_E;
            break;
        case 'H':
        case 'h':
            *port = GPIO_PORT_H;
            break;
        default:
            return STATUS_ERROR_PARAM;
    }

    return STATUS_OK;
}

/**
 * @brief Convert a pin string to a GPIO_Pin_t
 * 
 * @param[in] pin_str Pin string (e.g., "0", "1", "15")
 * @param[out] pin Pointer to store the pin
 * @return Status_t Operation status
 */
static Status_t ParsePin(const char* pin_str, GPIO_Pin_t* pin)
{
    long pin_val;
    char* end_ptr;

    if((pin_str == NULL) || (pin == NULL)){
        return STATUS_ERROR_NULL;
    }

    /* Convert string to long */
    pin_val = strtol(pin_str, &end_ptr, 10);

    /* Check for conversion errors (no digits consumed or trailing chars) */
    if((end_ptr == pin_str) || (*end_ptr != '\0')){
        return STATUS_ERROR_PARAM;
    }

    /* Check if pin is in valid range */
    if((pin_val < 0) || (pin_val >= GPIO_PIN_COUNT)){
        return STATUS_ERROR_PARAM;
    }

    *pin = (GPIO_Pin_t)pin_val;

    return STATUS_OK;
}

/**
 * @brief Convert a pin mode string to a GPIO_Mode_t
 * 
 * @param[in] mode_str Mode string (e.g., "input", "output")
 * @param[out] mode Pointer to store the mode
 * @return Status_t Operation status
 */
static Status_t ParseMode(const char* mode_str, GPIO_Mode_t* mode)
{
    if((mode_str == NULL) || (mode == NULL)){
        return STATUS_ERROR_NULL;
    }

    if(strcmp(mode_str, "input") == 0){
        *mode = GPIO_MODE_INPUT;
    }
    else if(strcmp(mode_str, "output") == 0){
        *mode = GPIO_MODE_OUTPUT;
    }
    else if(strcmp(mode_str, "alternate") == 0){
        *mode = GPIO_MODE_ALT;
    }
    else if(strcmp(mode_str, "analog") == 0){
        *mode = GPIO_MODE_ANALOG;
    }
    else{
        return STATUS_ERROR_PARAM;
    }

    return STATUS_OK;
}

/**
 * @brief Convert a pin state string to a GPIO_PinState_t
 *
 * @param[in] state_str State string ("1"/"high"/"on" or "0"/"low"/"off")
 * @param[out] state Pointer to store the state
 * @return Status_t Operation status
 */
static Status_t ParseState(const char* state_str, GPIO_PinState_t* state)
{
    if((state_str == NULL) || (state == NULL)){
        return STATUS_ERROR_NULL;
    }

    if((strcmp(state_str, "1") == 0) ||
       (strcmp(state_str, "high") == 0) ||
       (strcmp(state_str, "on") == 0)){
        *state = GPIO_PIN_SET;
    }
    else if((strcmp(state_str, "0") == 0) ||
            (strcmp(state_str, "low") == 0) ||
            (strcmp(state_str, "off") == 0)){
        *state = GPIO_PIN_RESET;
    }
    else{
        return STATUS_ERROR_PARAM;
    }

    return STATUS_OK;
}

/**
 * @brief Parse a command string into tokens
 * 
 * @param[in] command Command string to parse
 * @param[out] tokens Array to store tokens
 * @param[in] max_tokens Maximum number of tokens to parse
 * @return uint32_t Number of tokens parsed
 */
static uint32_t ParseCommandTokens(const char* command, char tokens[][GPIO_DEMO_MAX_LINE], uint32_t max_tokens)
{
    uint32_t token_count = 0U;
    uint32_t i = 0U;
    uint32_t token_start = 0U;
    uint32_t token_length = 0U;
    uint32_t command_length;

    if((command == NULL) || (tokens == NULL) || (max_tokens == 0U)){
        return 0U;
    }

    command_length = strlen(command);

    while((i < command_length) && (token_count < max_tokens)){
        /* Skip leading whitespace */
        while((i < command_length) && ((command[i] == ' ') || (command[i] == '\t'))){
            i++;
        }

        if(i >= command_length){
            break;
        }

        /* Mark start of token */        
        token_start = i;
        token_length = 0U;

        /* Find end of token */
        while((i < command_length) && (command[i] != ' ') && (command[i] != '\t')){
            i++;
            token_length++;
        }

        /* Copy token */
        if(token_length > 0U){
            if(token_length >= GPIO_DEMO_MAX_LINE){
                token_length = GPIO_DEMO_MAX_LINE - 1U;
            }

            strncpy(tokens[token_count], &command[token_start], token_length);
            tokens[token_count][token_length] = '\0';
            token_count++;
        }
    }

    return token_count;
}

/**
 * @brief Handle the init command
 * 
 * @param[in] token_count Number of tokens
 * @param[in] tokens Array of tokens
 * @return Status_t Operation status
 */
static Status_t HandleInitCommand(uint32_t token_count, char tokens[][GPIO_DEMO_MAX_LINE])
{
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    GPIO_Mode_t mode;
    GPIO_Config_t config;
    Status_t status;
    int32_t slot;

    /* Check token count */
    if(token_count != 4U){
        UART_Printf("Error: init command requires 3 parameters (port, pin, mode)\r\n");
        return STATUS_ERROR_PARAM;
    }

    /* Parse parameters */
    status = ParsePort(tokens[1], &port);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid port '%s'\r\n", tokens[1]);
        return status;
    }

    status = ParsePin(tokens[2], &pin);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid pin '%s'\r\n", tokens[2]);
        return status;
    }

    status = ParseMode(tokens[3], &mode);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid mode '%s'\r\n", tokens[3]);
        return status;
    }

    /* Check if pin is already configured */
    if(FindConfiguredPin(port, pin) >= 0){
        UART_Printf("Error: pin P%c%d is already configured\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Find a free slot for the new pin */
    slot = FindFreePinSlot();
    if(slot < 0){
        UART_Printf("Error: maximum number of pins (%d) already configured\r\n", MAX_DEMO_PINS);
        return STATUS_ERROR_RESOURCE;
    }

    /* Configure the pin */
    memset(&config, 0, sizeof(config));
    config.port = port;
    config.pin = pin;
    config.mode = mode;
    config.pull = GPIO_PULL_NONE;

    if(mode == GPIO_MODE_OUTPUT){
        config.otype = GPIO_OTYPE_PUSH_PULL;
        config.speed = GPIO_SPEED_MEDIUM;
    }

    status = GPIO_PinInit(&config);
    if(status != STATUS_OK){
        UART_Printf("Error: failed to initialize pin P%c%d (error %d)\r\n", 'A' + port, pin, status);
        return status;
    }

    /* Store the configuration */
    configured_pins[slot].configured = 1U;
    configured_pins[slot].port = port;
    configured_pins[slot].pin = pin;
    configured_pins[slot].mode = mode;

    UART_Printf("Pin P%c%d initialized as %s\r\n", 'A' + port, pin, tokens[3]);
    return STATUS_OK;
}

/**
 * @brief Handle the write command
 * 
 * @param[in] token_count Number of tokens
 * @param[in] tokens Array of tokens
 * @return Status_t Operation status
 */
static Status_t HandleWriteCommand(uint32_t token_count, char tokens[][GPIO_DEMO_MAX_LINE])
{
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    GPIO_PinState_t state;
    Status_t status;
    int32_t slot;

    /* Check tokeen count */
    if(token_count != 4U){
        UART_Printf("Error: write command requires 3 parameters (port, pin, state)\r\n");
        return STATUS_ERROR_PARAM;
    }

    /* Parse parameters */
    status = ParsePort(tokens[1], &port);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid port '%s'\r\n", tokens[1]);
        return status;
    }

    status = ParsePin(tokens[2], &pin);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid pin '%s'\r\n", tokens[2]);
        return status;
    }

    status = ParseState(tokens[3], &state);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid state '%s'\r\n", tokens[3]);
        return status;
    }

    /* Check if pin is configured */
    slot = FindConfiguredPin(port, pin);
    if(slot < 0){
        UART_Printf("Error: pin P%c%d is not configured\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Check if pin is configured as output */
    if(configured_pins[slot].mode != GPIO_MODE_OUTPUT){
        UART_Printf("Error: pin P%c%d is not configured as output\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Write the pin state */
    status = GPIO_WritePin(port, pin, state);
    if(status != STATUS_OK){
        UART_Printf("Error: failed to write pin P%c%d (error %d)\r\n", 'A' + port, pin, status);
        return status;
    }

    UART_Printf("Pin P%c%d set to %s\r\n", 'A' + port, pin, (state == GPIO_PIN_SET) ? "high" : "low");

    return STATUS_OK;
}


/**
 * @brief Handle the read command
 * 
 * @param[in] token_count Number of tokens
 * @param[in] tokens Array of tokens
 * @return Status_t Operations Status
 */
static Status_t HandleReadCommand(uint32_t token_count, char tokens[][GPIO_DEMO_MAX_LINE])
{
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    GPIO_PinState_t state;
    Status_t status;
    int32_t slot;

    /* Check token count */
    if(token_count != 3U){
        UART_Printf("Error: read command requires 2 parameters (port, pin)\r\n");
        return STATUS_ERROR_PARAM;
    }

    /* Parse parameters */
    status = ParsePort(tokens[1], &port);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid port '%s'\r\n", tokens[1]);
        return status;
    }

    status = ParsePin(tokens[2], &pin);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid pin '%s'\r\n", tokens[2]);
        return status;
    }

    /* Check if pin is configured */
    slot = FindConfiguredPin(port, pin);
    if(slot < 0){
        UART_Printf("Error: pin P%c%d is not configured\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Read the pin state */
    status = GPIO_ReadPin(port, pin, &state);
    if(status != STATUS_OK){
        UART_Printf("Error: failed to read pin P%c%d (error %d)\r\n", 'A' + port, pin, status);
        return status;
    }

    UART_Printf("Pin P%c%d reads %s\r\n", 'A' + port, pin, (state == GPIO_PIN_SET) ? "high" : "low");

    return STATUS_OK;
}

/**
 * @brief Handle the toggle command
 * 
 * @param[in] token_count Number of tokens
 * @param[in] tokens Array of tokens
 * @return Status_t Operation status
 */
static Status_t HandleToggleCommand(uint32_t token_count, char tokens[][GPIO_DEMO_MAX_LINE])
{
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    Status_t status;
    int32_t slot;

    /* Check token count */
    if(token_count != 3U){
        UART_Printf("Error: toggle command requires 2 parameters (port, pin)\r\n");
        return STATUS_ERROR_PARAM;
    }

    /* Parse parameters */
    status = ParsePort(tokens[1], &port);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid port '%s'\r\n", tokens[1]);
        return status;
    }

    status = ParsePin(tokens[2], &pin);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid pin '%s'\r\n", tokens[2]);
        return status;
    }

    /* Check if pin is configured */
    slot = FindConfiguredPin(port, pin);
    if(slot < 0){
        UART_Printf("Error: pin P%c%d is not configured\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Check if pin is configured as output */
    if(configured_pins[slot].mode != GPIO_MODE_OUTPUT){
        UART_Printf("Error: pin P%c%d is not configured as output\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Toggle the pin state */
    status = GPIO_TogglePin(port, pin);
    if(status != STATUS_OK){
        UART_Printf("Error: failed to toggle pin P%c%d (error %d)\r\n", 'A' + port, pin, status);
        return status;
    }

    UART_Printf("Pin P%c%d toggled\r\n", 'A' + port, pin);

    return STATUS_OK;
}

/**
 * @brief Handle the 'list' command
 * 
 * @return Status_t Operation status
 */
static Status_t HandleListCommand(void)
{
    uint32_t i;
    uint32_t count = 0U;

    UART_Printf("Configured pins:\r\n");

    for(i = 0U; i < MAX_DEMO_PINS; i++){
        if(configured_pins[i].configured != 0U){
            const char* mode_str;

            /* Convert mode to string */
            switch (configured_pins[i].mode)
            {
                case GPIO_MODE_INPUT:
                    mode_str = "input";
                    break;
                case GPIO_MODE_OUTPUT:
                    mode_str = "output";
                    break;
                case GPIO_MODE_ALT:
                    mode_str = "alternate";
                    break;
                case GPIO_MODE_ANALOG:
                    mode_str = "analog";
                    break;
                default:
                    mode_str = "unknown";
                    break;
            }

            UART_Printf("  %d. P%c%d: %s\r\n", count+1, 'A' + configured_pins[i].port,
                            configured_pins[i].pin, mode_str);
            count++;
        }
    }

    if(count == 0U){
        UART_Printf("  No pins configured\r\n");
    }

    return STATUS_OK;
}


/**
 * @brief Handle the deinit command
 * 
 * @param[in] token_count Number of tokens
 * @param[in] tokens Array of tokens
 * @return Status_t Operation status
 */
static Status_t HandleDeinitCommand(uint32_t token_count, char tokens[][GPIO_DEMO_MAX_LINE])
{
    GPIO_Port_t port;
    GPIO_Pin_t pin;
    Status_t status;
    int32_t slot;

    /* Check token count */
    if(token_count != 3U){
        UART_Printf("Error: deinit command requires 2 parameters (port, pin)\r\n");
        return STATUS_ERROR_PARAM;
    }

    /* Parse parameters */
    status = ParsePort(tokens[1], &port);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid port '%s'\r\n", tokens[1]);
        return status;
    }

    status = ParsePin(tokens[2], &pin);
    if(status != STATUS_OK){
        UART_Printf("Error: invalid pin '%s'\r\n", tokens[2]);
        return status;
    }

    /* Check if pin is configured */
    slot = FindConfiguredPin(port, pin);
    if(slot < 0){
        UART_Printf("Error: pin P%c%d is not configured\r\n", 'A' + port, pin);
        return STATUS_ERROR_PARAM;
    }

    /* Release the configuration tracking slot */
    configured_pins[slot].configured = 0U;

    UART_Printf("Pin P%c%d deinitialized\r\n", 'A' + port, pin);

    return STATUS_OK;
}