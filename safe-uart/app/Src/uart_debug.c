/**
 * @file uart_debug.h
 * @brief UART interface for debugging and demonstration
 * 
 * This file provides a simplified UART interface for debugging and
 * demonstration of the GPIO driver, with MISRA C compliant implementation.
 * 
 * @version 1.0
 */

#include "uart_debug.h"
#include "stm32f411xe.h"
#include <stdio.h>
#include <string.h>

/* UART buffer size */
#define UART_BUFFER_SIZE        (256U)

/* UART State tracking */
static uint8_t uart_initialized = 0;

/* Line buffer and state */
static char line_buffer[UART_BUFFER_SIZE];
static uint32_t line_pos = 0U;
static uint8_t line_complete = 0U;