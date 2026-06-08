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
