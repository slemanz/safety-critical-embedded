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
