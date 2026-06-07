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

/* USART2 pin mapping on GPIOA (PA2 = TX, PA3 = RX) */
#define UART_TX_PIN             (2U)    /* PA2 - USART2_TX */
#define UART_RX_PIN             (3U)    /* PA3 - USART2_RX */

/* GPIO register field widths */
#define UART_MODER_BITS         (2U)    /* Bits per pin in MODER register */
#define UART_AFR_BITS           (4U)    /* Bits per pin in AFR register */

/* GPIO field values / masks */
#define UART_MODE_ALTERNATE     (2U)    /* Alternate function mode (10b) */
#define UART_MODE_MASK          (3UL)   /* MODER field mask (2 bits) */
#define UART_AF7                (7U)    /* Alternate function 7 (USART2) */
#define UART_AFR_MASK           (0xFUL) /* AFR field mask (4 bits) */

/* UART State tracking */
static uint8_t uart_initialized = 0;

/* Line buffer and state */
static char line_buffer[UART_BUFFER_SIZE];
static uint32_t line_pos = 0U;
static uint8_t line_complete = 0U;

/**
 * @brief Configure USART2 pins (PA2/PA3)
 * 
 * @return Status_t Operation status
 */
static Status_t UART_ConfigurePins(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set alternate function 7 (USART2) for PA2 */
    GPIOA->AFR[0] &= ~(UART_AFR_MASK << (UART_TX_PIN * UART_AFR_BITS));
    GPIOA->AFR[0] |= ((uint32_t)UART_AF7 << (UART_TX_PIN * UART_AFR_BITS));

    /* Set alternate function 7 (USART2) for PA3 */
    GPIOA->AFR[0] &= ~(UART_AFR_MASK << (UART_RX_PIN * UART_AFR_BITS));
    GPIOA->AFR[0] |= ((uint32_t)UART_AF7 << (UART_RX_PIN * UART_AFR_BITS));

    /* Configure PA2 (TX) - alternate function mode */
    GPIOA->MODER &= ~(UART_MODE_MASK << (UART_TX_PIN * UART_MODER_BITS));
    GPIOA->MODER |= ((uint32_t)UART_MODE_ALTERNATE << (UART_TX_PIN * UART_MODER_BITS));

    /* Configure PA3 (RX) - alternate function mode */
    GPIOA->MODER &= ~(UART_MODE_MASK << (UART_RX_PIN * UART_MODER_BITS));
    GPIOA->MODER |= ((uint32_t)UART_MODE_ALTERNATE << (UART_RX_PIN * UART_MODER_BITS));

    return STATUS_OK;
}


Status_t UART_Init(const UART_Config_t* config)
{
    uint32_t parity_config;
    uint32_t stop_bits_config;
    uint32_t word_length;
    uint32_t baud_div;

    /* Validate input */
    if(config == NULL){
        return STATUS_ERROR_NULL;
    }

    /* Validate parameters */
    if((config->data_bits != 7U && config->data_bits != 8U) ||
       (config->stop_bits != 1U && config->stop_bits != 2U) ||
       (config->parity > 2U)) {
            return STATUS_ERROR_PARAM;
       }

    /* Configure GPIO pins for UART */
    UART_ConfigurePins();

    /* Enable USART2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Disable USART2 before configuration */
    USART2->CR1 &= ~USART_CR1_UE;

    /* Configure parity */
    if(config->parity == 0U){
        /* No parity */
        parity_config = 0U;
    }else{
        /* Enable parity control */
        parity_config = USART_CR1_PCE;
    }
}