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

/**
 * @brief Initialize the UART debug interface
 * 
 * @param[in] config Pointer to UART configuration
 * @return Status_t Operation status
 * @retval STATUS_OK UART initialized succesfully
 * @retval STATUS_ERROR_NULL Null pointer provided
 * @retval STATUS_ERROR_PARAM Invalid parameter
 */
Status_t UART_Init(const UART_Config_t* config);

/**
 * @brief Send a string over uart
 * 
 * @param[in] str Null-terminated string to send
 * @return Status_t Operation status
 * @retval STATUS_OK String sent successfully
 * @retval STATUS_ERROR_NULL Null pointer provided
 * @retval STATUS_ERROR_INIT UART not initialized
 */
Status_t UART_SendString(const char* str);

/**
 * @brief Send a string over UART (printf-style)
 * 
 * @param[in] format Format string (printf-style)
 * @param[in] ... Variable arguments
 * @return Status_t Operation status
 * @retval STATUS_OK String sent successfully
 * @retval STATUS_ERROR_NULL Null pointer provided
 * @retval STATUS_ERROR_INIT UART not initialized
 * @retval STATUS_ERROR_GENERIC String formatting failed
 */
Status_t UART_Printf(const char* format, ...);

/**
 * @brief Check if a character is available to read
 * 
 * @return uint8_t 1 if character available, 0 otherwise
 */
uint8_t UART_IsCharAvailable(void);

/**
 * @brief Read a character from UART
 * 
 * @param[out] ch Pointer to store the received character
 * @return Status_t Operation status
 * @retval STATUS_OK Character read succesfully
 * @retval STATUS_ERROR_NULL Null pointer provided
 * @retval STATUS_ERROR_INIT UART not initialized
 * @retval STATUS_PENDING No character available
 */
Status_t UART_GetChar(char* ch);

/**
 * @brief Read a line (until newline or buffer full)
 * 
 * @param[out] buffer Buffer to store the line
 * @param[in] max_len Maximum buffer length
 * @return Status_t Operation status
 * @retval STATUS_OK Line read successfully
 * @retval STATUS_ERROR_NULL Null pointer provided
 * @retval STATUS_ERROR_PARAM Invalid parameter
 * @retval STATUS_ERROR_INIT UART not initialized
 * @retval STATUS_PENDING Line reading in progress
 */
Status_t UART_GetLine(char* buffer, uint32_t max_len);

/**
 * @brief Deinitialize the UART debug interface
 * 
 * @return Status_t Operation status
 * @retval STATUS_OK UART deinitialized successfully
 * @retval STATUS_ERROR_INIT UART not initialized
 */
Status_t UART_DeInit(void);


#endif /* UART_DEBUG_H_ */