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