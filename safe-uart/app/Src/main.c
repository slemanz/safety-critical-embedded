/**
 * @file main.c
 * @brief Reference application demonstrating professional use of the uart_debug module
 *
 * This example shows the intended usage pattern of the uart_debug module in a
 * safety-aware style, consistent with the practices documented in this repository
 * (MISRA C alignment, fault tolerance and fail-safe behaviour, explicit status
 * handling on every call):
 *
 *  - Status-checked initialisation of the console; the system never proceeds
 *    without a working UART.
 *  - A fail-safe handler that releases the peripheral and halts deterministically
 *    when the console reports an unrecoverable error.
 *  - A non-blocking main loop: line assembly returns STATUS_PENDING until a full
 *    line is available, leaving room for other periodic work.
 *  - Output produced exclusively through the module's own UART_Printf /
 *    UART_SendString (no reliance on the C library's printf, see README).
 *
 * Behaviour: the application prints a banner and then acts as a tiny line-based
 * console. Each line typed by the user is read with UART_GetLine (which echoes
 * the input as it is typed) and answered with a one-line response.
 *
 * Hardware mapping (traceability):
 *  - PA2 : USART2_TX - serial console transmit.
 *  - PA3 : USART2_RX - serial console receive.
 *  Console settings: 115200 baud, 8 data bits, no parity, 1 stop bit (8N1).
 */

#include <stdint.h>
#include <string.h>
#include "fpu.h"
#include "uart_debug.h"

/* -------------------------------------------------------------------------- */
/* Application configuration                                                   */
/* -------------------------------------------------------------------------- */

/* Console line settings, kept as named constants for traceability */
#define CONSOLE_BAUDRATE        (115200U)
#define CONSOLE_DATA_BITS       (8U)
#define CONSOLE_STOP_BITS       (1U)
#define CONSOLE_PARITY          (0U)    /* 0 = none */

/* Caller-side line buffer. Bounded at compile time - no dynamic memory. */
#define LINE_MAX_LEN            (64U)

/* Static console configuration (statically allocated - no dynamic memory) */
static const UART_Config_t console_config = {
    .baudrate  = CONSOLE_BAUDRATE,
    .data_bits = CONSOLE_DATA_BITS,
    .stop_bits = CONSOLE_STOP_BITS,
    .parity    = CONSOLE_PARITY
};

/* -------------------------------------------------------------------------- */
/* Fail-safe handling                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Drive the system into a controlled safe state after a fatal fault
 *
 * Implements the fail-safe principle from the documentation: on an
 * unrecoverable console fault the application releases the peripheral and halts
 * deterministically. In a production system a watchdog would subsequently force
 * a recovery reset. No message is printed here on purpose - the very channel we
 * would use to report the fault is the one that failed.
 */
static void app_safe_state(void)
{
    /* Best-effort release of the UART peripheral */
    (void)UART_DeInit();

    /* Controlled halt; awaiting watchdog / operator intervention */
    for(;;)
    {
        /* Intentionally empty */
    }
}

/* -------------------------------------------------------------------------- */
/* Command handling                                                            */
/* -------------------------------------------------------------------------- */

/**
 * @brief Produce a one-line response for a completed input line
 *
 * @param[in] line Null-terminated line received from the console
 */
static void app_handle_line(const char* line)
{
    if(strcmp(line, "help") == 0)
    {
        (void)UART_SendString("commands: help, ping\r\n");
    }
    else if(strcmp(line, "ping") == 0)
    {
        (void)UART_SendString("pong\r\n");
    }
    else
    {
        /* UART_Printf demonstrates safe, bounded, status-returning formatting */
        (void)UART_Printf("echo: %s\r\n", line);
    }
}

/* -------------------------------------------------------------------------- */
/* Application entry point                                                      */
/* -------------------------------------------------------------------------- */

int main(void)
{
    char line[LINE_MAX_LEN];
    Status_t status;

    fpu_enable();

    /* Bring up the console. Without it there is nothing to demonstrate and no
     * channel to report on, so a failure goes straight to the safe state. */
    if(UART_Init(&console_config) != STATUS_OK)
    {
        app_safe_state();
    }

    (void)UART_SendString("\r\nsafe-uart demo ready\r\n");
    (void)UART_SendString("type 'help' and press enter\r\n> ");

    for(;;)
    {
        /* Non-blocking: returns STATUS_PENDING until a full line is assembled */
        status = UART_GetLine(line, sizeof(line));

        if(status == STATUS_OK)
        {
            /* Ignore empty lines (e.g. the LF of a CR/LF terminal sequence) */
            if(line[0] != '\0')
            {
                app_handle_line(line);
                (void)UART_SendString("> ");
            }
        }
        else if(status == STATUS_PENDING)
        {
            /* No complete line yet; other periodic work could run here. */
        }
        else
        {
            /* Any other status from the console is unexpected and fatal. */
            app_safe_state();
        }
    }
}
