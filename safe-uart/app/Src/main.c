/**
 * @file main.c
 * @brief Reference application demonstrating professional use of the safe_gpio driver
 *
 * This example shows the intended usage pattern of the safe_gpio module in a
 * safety-aware style, consistent with the practices documented in this repository
 * (MISRA C alignment, fault tolerance and fail-safe behaviour, traceable mapping
 * between requirements and code):
 *
 *  - Centralised, status-checked initialisation of every GPIO resource.
 *  - A fail-safe error handler that drives the system into a known safe state
 *    when any GPIO operation reports an error (see "considerations" / "keys").
 *  - Deterministic, tick-based periodic execution (no busy-wait, bounded loop).
 *  - Strongly-typed, validated GPIO read/write operations with explicit status
 *    handling on every call.
 *
 * Hardware mapping (traceability):
 *  - PC13 : status LED       - output, blinks at LED_BLINK_PERIOD_TICKS (the "blink").
 *  - PA0  : user button      - input with pull-up, active-low.
 *  - PB3  : actuator output  - driven active while the button is pressed.
 *
 * Timebase note: one tick equals one second (SysTick reload = core clock).
 */

#include <stdint.h>
#include <stdio.h>
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "safe_gpio.h"

/* -------------------------------------------------------------------------- */
/* Application configuration                                                   */
/* -------------------------------------------------------------------------- */

/* Pin mapping - kept as named constants to preserve traceability */
#define LED_PORT                (GPIO_PORT_C)
#define LED_PIN                 (GPIO_PIN_13)
#define BUTTON_PORT             (GPIO_PORT_A)
#define BUTTON_PIN              (GPIO_PIN_0)
#define ACTUATOR_PORT           (GPIO_PORT_B)
#define ACTUATOR_PIN            (GPIO_PIN_3)

/* Periodic task intervals, expressed in timebase ticks (1 tick = 1 s) */
#define LED_BLINK_PERIOD_TICKS  (1U)

/* -------------------------------------------------------------------------- */
/* Static pin configurations (statically allocated - no dynamic memory)        */
/* -------------------------------------------------------------------------- */

static const GPIO_Config_t led_config = {
    .port      = LED_PORT,
    .pin       = LED_PIN,
    .mode      = GPIO_MODE_OUTPUT,
    .otype     = GPIO_OTYPE_PUSH_PULL,
    .speed     = GPIO_SPEED_LOW,
    .pull      = GPIO_PULL_NONE,
    .alternate = 0U
};

static const GPIO_Config_t button_config = {
    .port      = BUTTON_PORT,
    .pin       = BUTTON_PIN,
    .mode      = GPIO_MODE_INPUT,
    .otype     = GPIO_OTYPE_PUSH_PULL,
    .speed     = GPIO_SPEED_LOW,
    .pull      = GPIO_PULL_UP,
    .alternate = 0U
};

static const GPIO_Config_t actuator_config = {
    .port      = ACTUATOR_PORT,
    .pin       = ACTUATOR_PIN,
    .mode      = GPIO_MODE_OUTPUT,
    .otype     = GPIO_OTYPE_PUSH_PULL,
    .speed     = GPIO_SPEED_LOW,
    .pull      = GPIO_PULL_NONE,
    .alternate = 0U
};

/* -------------------------------------------------------------------------- */
/* Fail-safe handling                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Drive the system into a controlled safe state after a fatal fault
 *
 * Implements the fail-safe principle from the documentation: on an
 * unrecoverable fault the application reports the cause, releases the GPIO
 * outputs to their reset state, and halts deterministically. In a production
 * system a watchdog would subsequently force a recovery reset.
 *
 * @param[in] reason Human-readable description of the originating fault
 */
static void app_safe_state(const char* reason)
{
    printf("FATAL: %s -- entering safe state\r\n", reason);

    /* Best-effort release of all GPIO outputs to their reset state */
    (void)GPIO_DeInit();

    /* Controlled halt; awaiting watchdog / operator intervention */
    for(;;)
    {
        /* Intentionally empty */
    }
}

/* -------------------------------------------------------------------------- */
/* GPIO bring-up                                                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initialise the GPIO driver and every pin used by the application
 *
 * Every call is status-checked; any failure is escalated to the fail-safe
 * handler so the system never runs with a partially configured I/O set.
 */
static void app_gpio_init(void)
{
    if(GPIO_Init() != STATUS_OK)
    {
        app_safe_state("GPIO_Init");
    }

    if(GPIO_PinInit(&led_config) != STATUS_OK)
    {
        app_safe_state("LED pin init");
    }

    if(GPIO_PinInit(&button_config) != STATUS_OK)
    {
        app_safe_state("Button pin init");
    }

    if(GPIO_PinInit(&actuator_config) != STATUS_OK)
    {
        app_safe_state("Actuator pin init");
    }

    /* Define the initial output state explicitly (no implicit start-up state) */
    if(GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_PIN, GPIO_PIN_RESET) != STATUS_OK)
    {
        app_safe_state("Actuator initial state");
    }
}

/* -------------------------------------------------------------------------- */
/* Application entry point                                                      */
/* -------------------------------------------------------------------------- */

int main(void)
{
    uint8_t v_major = 0U;
    uint8_t v_minor = 0U;
    uint8_t v_patch = 0U;
    uint32_t last_blink_tick;

    fpu_enable();
    debug_uart_init();
    timebase_init();

    app_gpio_init();

    if(GPIO_GetVersion(&v_major, &v_minor, &v_patch) == STATUS_OK)
    {
        printf("safe_gpio driver v%u.%u.%u ready\r\n",
               (unsigned int)v_major, (unsigned int)v_minor, (unsigned int)v_patch);
    }

    printf("Hello world!\r\n");
    printf("Debug is on!\r\n");

    last_blink_tick = tick_get();

    while(1)
    {
        uint32_t now = tick_get();

        /* Periodic status LED blink (the "blink") */
        if((now - last_blink_tick) >= LED_BLINK_PERIOD_TICKS)
        {
            last_blink_tick = now;

            if(GPIO_TogglePin(LED_PORT, LED_PIN) != STATUS_OK)
            {
                app_safe_state("LED toggle");
            }
        }

        /* Drive the actuator (PB3) from the button state.
         * The button is active-low, so a pressed button reads GPIO_PIN_RESET. */
        {
            GPIO_PinState_t button_state = GPIO_PIN_SET;

            if(GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN, &button_state) != STATUS_OK)
            {
                app_safe_state("Button read");
            }

            if(button_state == GPIO_PIN_RESET)
            {
                if(GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_PIN, GPIO_PIN_SET) != STATUS_OK)
                {
                    app_safe_state("Actuator set");
                }
            }
            else
            {
                if(GPIO_WritePin(ACTUATOR_PORT, ACTUATOR_PIN, GPIO_PIN_RESET) != STATUS_OK)
                {
                    app_safe_state("Actuator reset");
                }
            }
        }
    }
}
