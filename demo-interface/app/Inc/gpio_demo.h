/** 
 * @file gpio_demo.h
 * @brief GPIO driver demonstration interface
 * 
 * This file provides a demonstration interface for the safe GPIO driver
 * allowing interaction via UART commnads
 * 
 * @version 1.0
 */
#ifndef INC_GPIO_DEMO_H
#define INC_GPIO_DEMO_H

#include "safe_status.h"

/**
 * @brief Maximum line length for commands
 */
#define GPIO_DEMO_MAX_LINE      (64U)


/**
 * @brief Initialize the GPIO demonstration
 * 
 * @return Status_t Operation status
 * @retval STATUS_OK Initialization successful
 * @retval STATUS_ERROR_INIT Initialization failed
 */
Status_t GPIO_Demo_Init(void);

/**
 * @brief Run the GPIO demonstration
 * 
 * This function should be called periodically to process user commands.
 * 
 * @return Status_t Operation status
 * @retval STATUS_OK Processing successful
 * @retval STATUS_ERROR_INIT Demo not initialized
 */
Status_t GPIO_Demo_Run(void);

/**
 * @brief Display help for available commands
 * 
 * @return Status_t Operation status
 * @retval STATUS_OK Processing successful
 * @retval STATUS_ERROR_INIT Demo not initialized
 */
Status_t GPIO_Demo_DisplayHelp(void);

/**
 * @brief Proccess a command from the user
 * 
 * @return Status_t Operation status
 * @retval STATUS_OK Processing successful
 * @retval STATUS_ERROR_INIT Demo not initialized
 * @retval STATUS_ERROR_PARAM Invalid command
 */
Status_t GPIO_Demo_ProccessCommand(const char* command);

#endif /* INC_GPIO_DEMO_H */