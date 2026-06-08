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
