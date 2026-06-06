/**
 * @file uart_debug.h
 * @brief UART interface for debugging and demonstration
 * 
 * This file provides a simplified UART interface for debugging and
 * demonstration of the GPIO driver, with MISRA C compliant implementation.
 * 
 * @version 1.0
 */
#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

#include <stdint.h>
#include "safe_status.h"

/**
 * @brief UART configuration structure
 */
typedef struct{
    uint32_t baudrate;  /**< Baud rate (e.g., 9600, 115200) */
    uint8_t data_bits;  /**< Data bits */
    uint8_t stop_bits;  /**< Stop bits (1 or 2) */
    uint8_t parity;     /**< Parity (0 = none, 1 = odd, 2 = even) */
}UART_Config_t;


#endif /* UART_DEBUG_H_ */