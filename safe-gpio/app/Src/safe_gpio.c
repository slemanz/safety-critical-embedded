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