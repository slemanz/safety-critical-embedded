/**
 * @file safe_gpio.c
 * @brief MISRA-compliant GPIO driver implementation
 * 
 * This file implements a type-safe GPIO driver that follows
 * MISRA C guidelines for memory-safe register access.
 * 
 * @version 1.0
 */

 #include <stddef.h>
 #include "stm32f411xe.h"
 #include "safe_gpio.h"

 /* Driver version information */
 #define GPIO_VERSION_MAJOR     (1U)
 #define GPIO_VERSION_MINOR     (0U)
 #define GPIO_VERSION_PATCH     (0U)

 /* Internal constants */
 #define GPIO_MODE_BITS         (2U)    /* Bits per pin in MODE register */
 #define GPIO_OTYPE_BITS        (1U)    /* Bits per pin in OTYPE register */
 #define GPIO_OSPEED_BITS       (2U)    /* Bits per pin in OSPEED register */
 #define GPIO_PUPD_BITS         (2U)    /* Bits per pin in PUPD register */
 #define GPIO_AFR_BITS          (4U)    /* Bits per pin in AFR register */

 
 /* Bitmask for a single pin */
 #define GPIO_PIN_MASK(pin)     (1UL << (pin))

 /* Initialization tracking */
 static uint8_t gpio_initialized = 0U;

 /* Pin configuration tracking - for validation of operations */
 static uint16_t gpio_output_pins[GPIO_PORT_COUNT] = {0U};
 static uint16_t gpio_initialized_pins[GPIO_PORT_COUNT] = {0U};

 /**
  * @brief Access the appropriate GPIO register safely
  * 
  * Safely converts the port enum to the actual peripheral register base address.
  * This function provides controlled access to memory-mapped registers.
  * 
  * @param[in] port The GPIO port
  * @return GPIO_TypeDef* Pointer to the corresponding GPIO register block
  */
 static GPIO_TypeDef* GPIO_GetPortRegister(GPIO_Port_t port)
 {
    GPIO_TypeDef *gpio_regs = NULL;

    switch(port){
        case GPIO_PORT_A:
            gpio_regs = GPIOA;
            break;
        case GPIO_PORT_B:
            gpio_regs = GPIOB;
            break;
        case GPIO_PORT_C:
            gpio_regs = GPIOC;
            break;
        case GPIO_PORT_D:
            gpio_regs = GPIOD;
            break;
        case GPIO_PORT_E:
            gpio_regs = GPIOE;
            break;
        case GPIO_PORT_H:
            gpio_regs = GPIOH;
            break;
        default:
            /* Invalid port, return NULL */
            gpio_regs = NULL;
            break;
    }

    return gpio_regs;
 }

 /**
  * @brief Check if a GPIO port is valid
  * 
  * @param[in] port The GPIO port to check
  * @return bool True if valid, false otherwise
  */
static bool GPIO_IsValidPort(GPIO_Port_t port)
{
    return (port < GPIO_PORT_COUNT) && (port != GPIO_PORT_RESERVERD1) && (port != GPIO_PORT_RESERVERD2);
}

/**
 * @brief Check if a GPIO pin is valid
 * 
 * @param[in] pin The GPIO pin to check
 * @return bool True if valid, false otherwise
 */
static bool GPIO_IsValidPin(GPIO_Pin_t pin)
{
    return (pin < GPIO_PIN_COUNT);
}

/**
 * @brief Check if an alternate function is valid
 * 
 * @param[in] alternate The alternate function is valid
 * @return bool True if valid, false otherwise
 */
static bool GPIO_IsValidAlternate(uint8_t alternate)
{
    return (alternate <= 15U);
}

/**
 * @brief Enable the clock for a specific GPIO port
 * 
 * @param[in] port The GPIO port
 * @return Status_t Operation status
 */
static Status_t GPIO_EnableClock(GPIO_Port_t port)
{
    Status_t status = STATUS_OK;

    if(!GPIO_IsValidPort(port)){
        status = STATUS_ERROR_PARAM;
    }else{
        // enable the corresponding GPIO port clock in RCC
        switch (port){
            case GPIO_PORT_A:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
                break;
            case GPIO_PORT_B:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
                break;
            case GPIO_PORT_C:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
                break;
            case GPIO_PORT_D:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
                break;
            case GPIO_PORT_E:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
                break;
            case GPIO_PORT_H:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
                break;
            default:
                /* This shouldnt happen due to prior validation */
                status = STATUS_ERROR_PARAM;
                break;
        }
    }

    return status;
}

Status_t GPIO_Init(void)
{
    Status_t status = STATUS_OK;
    uint32_t i = 0U;

    /* Initialize tracking arrays */
    for(i = 0U; i < GPIO_PORT_COUNT; i++)
    {
        gpio_output_pins[i] = 0U;
        gpio_initialized_pins[i] = 0U;
    }

    /* Mark driver as initialized */
    gpio_initialized = 1U;

    return status;
}

Status_t GPIO_PinInit(const GPIO_Config_t* config)
{
    /* Some local variables */
    Status_t status = STATUS_OK;
    GPIO_TypeDef* gpio_regs;
    uint32_t pin_index;
    uint32_t reg_value;
    uint32_t temp;

    /* Validate inputs */
    if(config == NULL){
        return STATUS_ERROR_PARAM;
    }

    if(!GPIO_IsValidPort(config->port) || !GPIO_IsValidPin(config->pin)){
        return STATUS_ERROR_PARAM;
    }

    /* Check if alternate function is valid when in alternate mode */
    if((config->mode == GPIO_MODE_ALT) && 
        !GPIO_IsValidAlternate(config->alternate)){
        return STATUS_ERROR_PARAM;
    }

    /* Check if driver is initialized */
    if(gpio_initialized == 0U){
        return STATUS_ERROR_INIT;
    }

    /* Enable the GPIO port clock */
    status = GPIO_EnableClock(config->port);
    if(status != STATUS_OK){
        return status;
    }

    /* Get the GPIO port register */
    gpio_regs = GPIO_GetPortRegister(config->port);
    if(gpio_regs == NULL){
        return STATUS_ERROR_PARAM;
    }

    /* Configure the GPIO pin mode (input, output, alternate, analog) */
    pin_index = (uint32_t)config->pin;
    reg_value = gpio_regs->MODER;
    reg_value &= ~(3UL << (pin_index * GPIO_MODE_BITS)); /* clear mode bits */
    reg_value |= ((uint32_t)config->mode << (pin_index * GPIO_MODE_BITS)); /* Set new mode */
    gpio_regs->MODER = reg_value;

    /* Configure the output type (push-pull or open-drain)*/
    reg_value = gpio_regs->OTYPER;
    reg_value &= ~(1UL << pin_index); /* Clear output type bit */
    reg_value |= ((uint32_t)config->otype << pin_index); /* Set new output type */
    gpio_regs->OTYPER = reg_value;

    /* Configure the output speed */
    reg_value = gpio_regs->OSPEEDR;
    reg_value &= ~(3UL << (pin_index * GPIO_OSPEED_BITS)); /* Clear output speed bits */
    reg_value |= ((uint32_t)config->speed << (pin_index * GPIO_OSPEED_BITS)); /* Set new output speed */
    gpio_regs->OSPEEDR = reg_value;

    /* Configure the pull-up/pull-down resistors */
    reg_value = gpio_regs->PUPDR;
    reg_value &= ~(3UL << (pin_index * GPIO_PUPD_BITS)); /* Clear pull-up/pull-down bits */
    reg_value |= ((uint32_t)config->pull << (pin_index * GPIO_PUPD_BITS)); /* Set new pull-up/pull-down */
    gpio_regs->PUPDR = reg_value;

    /* Configure the alternate function */
    if(config->mode == GPIO_MODE_ALT){
        /* Select the appropriate alternate function register (AFR[0] to pins 0-7, AFR[1] for pins 8-15) */
        if(pin_index < 8U){
            temp = 0U;
        }else{
            temp = 1U;
            pin_index -= 8U;
        }

        reg_value = gpio_regs->AFR[temp];
        reg_value &= ~(0xFUL << (pin_index * GPIO_AFR_BITS)); /* Clear alternate function bits */
        reg_value |= ((uint32_t)config->alternate << (pin_index * GPIO_AFR_BITS)); /* Set new alternate function */
        gpio_regs->AFR[temp] = reg_value;
    }

    /* Update tracking for initialized pins */
    gpio_initialized_pins[config->port] |= GPIO_PIN_MASK(config->pin);

    /* Update tracking for output pins */
    if(config->mode == GPIO_MODE_OUTPUT){
        gpio_output_pins[config->port] |= GPIO_PIN_MASK(config->pin);
    }else{
        gpio_output_pins[config->port] &= ~GPIO_PIN_MASK(config->pin);
    }

    return STATUS_OK;
}

Status_t GPIO_WritePin(GPIO_Port_t port, GPIO_Pin_t pin, GPIO_PinState_t state)
{
    GPIO_TypeDef* gpio_regs;
    uint32_t pin_mask;

    /* Validate inputs */
    if(!GPIO_IsValidPort(port) || !(GPIO_IsValidPin(pin))){
        return STATUS_ERROR_INIT;
    }

    /* Check if the pin is initialized as output */
    if((gpio_output_pins[port] & GPIO_PIN_MASK(pin)) == 0U){
        return STATUS_ERROR_PARAM;
    }

    /* Get the GPIO port register */
    gpio_regs = GPIO_GetPortRegister(port);
    if(gpio_regs == NULL){
        return STATUS_ERROR_PARAM;
    }

    /* Create pin mask */
    pin_mask = GPIO_PIN_MASK(pin);

    /* Set or reset the pin based on requested state */
    if(state == GPIO_PIN_SET){
        gpio_regs->BSRR = pin_mask;
    }else{
        gpio_regs->BSRR = (pin_mask << 16);
    }
}