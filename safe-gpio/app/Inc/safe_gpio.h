/**
 * @file safe_gpio.h
 * @brief MISRA-compliant GPIO driver with strong type safety
 * 
 * This file provides a type-safe GPIO driver implementation that
 * follows MISRA C guidelines for memory-safe register access.
 * 
 * Key safety features:
 * - Strong typing for GPIO ports and pins
 * - Compile-time pin configuration validation
 * - Safe register access methods
 * - Input validation for all functions
 * 
 * @version 1.0
 */
#ifndef INC_SAFE_GPIO_H
#define INC_SAFE_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief GPIO port identifiers with strong typing
 */
typedef enum{
    GPIO_PORT_A = 0U,
    GPIO_PORT_B = 1U,
    GPIO_PORT_C = 2U,
    GPIO_PORT_D = 3U,
    GPIO_PORT_E = 4U,
    GPIO_PORT_H = 7U,
    GPIO_PORT_COUNT
}GPIO_Port_t;


/**
 * @brief GPIO pins identifiers with strong typing
 */
typedef enum{
    GPIO_PIN_0 = 0U,
    GPIO_PIN_1 = 1U,
    GPIO_PIN_2 = 2U,
    GPIO_PIN_3 = 3U,
    GPIO_PIN_4 = 4U,
    GPIO_PIN_5 = 5U,
    GPIO_PIN_6 = 6U,
    GPIO_PIN_7 = 7U,
    GPIO_PIN_8 = 8U,
    GPIO_PIN_9 = 9U,
    GPIO_PIN_10 = 10U,
    GPIO_PIN_11 = 11U,
    GPIO_PIN_12 = 12U,
    GPIO_PIN_13 = 13U,
    GPIO_PIN_14 = 14U,
    GPIO_PIN_15 = 15U,
    GPIO_PIN_COUNT
}GPIO_Pin_t;


/**
 * @brief GPIO pin mode options
 */
typedef enum{
    GPIO_MODE_INPUT  = 0U,      /**< Input mode */
    GPIO_MODE_OUTPUT = 1U,      /**< Output mode */
    GPIO_MODE_ALT    = 2U,      /**< Alternate function mode */
    GPIO_MODE_ANALOG = 3U       /**< Analog mode */
}GPIO_Mode_t;


/**
 * @brief GPIO output type options
 */
typedef enum{
    GPIO_OTYPE_PUSH_PULL = 0U,  /**< Push-pull output */
    GPIO_OTYPE_OPEN_DRAIN = 1U, /**< Open-drain output */
}GPIO_OutputType_t;


/**
 * @brief GPIO output speed options
 */
typedef enum{
    GPIO_SPEED_LOW       = 0U,
    GPIO_SPEED_MEDIUM    = 1U,
    GPIO_SPEED_HIGH      = 2U,
    GPIO_SPEED_VERY_HIGH = 3U
}GPIO_Speed_t;


/**
 * @brief GPIO pull-up/pull-down register options
 */
typedef enum{
    GPIO_PULL_NONE = 0U, /**< No pull-up or pull-down */
    GPIO_PULL_UP   = 1U, /**< Pull-up */
    GPIO_PULL_DOWN = 2U, /**< Pull-down */
}GPIO_Pull_t;

#endif /* INC_SAFE_GPIO_H */