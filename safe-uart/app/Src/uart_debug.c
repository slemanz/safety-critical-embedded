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
#include <stdarg.h>

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
       (config->parity > 2U) ||
       (config->baudrate == 0U)) {
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

        if(config->parity == 1U){
            /* Odd parity */
            parity_config |= USART_CR1_PS;
        }
    }

    /* Configure data bits */
    if(config->data_bits == 8U){
        if(config->parity == 0U){
            /* 8 data bits, no parity */
            word_length = 0U;
        }else{
            /* 8 data bits + parity = 9 bits */
            word_length = USART_CR1_M;
        }
    }else{
        /* 7 data bits */
        if(config->parity == 0U){
            /* 7 data bits, no parity */
            word_length = 0U;
        }else{
            /* 7 data bits + parity = 8*/
            word_length = 0U;
        }
    }

    /* Configure stop bits */
    if(config->stop_bits == 1U){
        stop_bits_config = 0U;
    }else{
        stop_bits_config = USART_CR2_STOP_1;
    }

    /* Calculate baud rate divider (assumming 16 MHz PCLK1)*/
    baud_div = (16000000U + (config->baudrate / 2U))/config->baudrate;

    /* Configure USART2 */
    USART2->CR1 = parity_config | word_length | USART_CR1_RE | USART_CR1_TE;
    USART2->CR2 = stop_bits_config;
    USART2->BRR = baud_div;

    /* Enable USART2 */
    USART2->CR1 |= USART_CR1_UE;

    /* Initialize line buffer */
    memset(line_buffer, 0, sizeof(line_buffer));
    line_pos = 0;
    line_complete = 0U;

    /* Mark as initialized */
    uart_initialized = 1U;

    return STATUS_OK;
}

Status_t UART_SendString(const char* str)
{
    uint32_t i;

    /* Validate input */
    if(str == NULL){
        return STATUS_ERROR_NULL;
    }

    /* Check if UART is initialized */
    if(uart_initialized == 0U){
        return STATUS_ERROR_INIT;
    }

    /* Send each character in the string */
    for(i = 0U; str[i] != '\0'; i++){
        /* wait for transmit buffer to be empty */
        while((USART2->SR & USART_SR_TXE) == 0U){
            /* Do nothing */
        }

        /* Send the character */
        USART2->DR = (uint32_t)(str[i] & 0xFFU);
    }

    /* Wait for transmission to complete */
    while((USART2->SR & USART_SR_TC) == 0U){
        /* Do nothing */
    }

    return STATUS_OK;
}

Status_t UART_Printf(const char* format, ...)
{
    char buffer[UART_BUFFER_SIZE];
    va_list args;
    Status_t status;
    int written;

    /* Validate input */
    if(format == NULL){
        return STATUS_ERROR_NULL;
    }

    /* Check if UART is initialized */
    if(uart_initialized == 0U){
        return STATUS_ERROR_INIT;
    }

    /* Format the string */
    va_start(args, format);
    written = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    /* Encoding error -> buffer content is indeterminate, do not transmit */
    if(written < 0){
        return STATUS_ERROR_GENERIC;
    }

    /* Send the formatted string */
    status = UART_SendString(buffer);

    return status;
}

uint8_t UART_IsCharAvailable(void)
{
    /* Check if UART is initialized */
    if(uart_initialized == 0U){
        return 0U;
    }

    /* Check if receive data buffer is not empty */
    if((USART2->SR & USART_SR_RXNE) != 0U){
        return 1U;
    }else{
        return 0U;
    }
}

Status_t UART_GetChar(char* ch)
{
    /* Validate input */
    if(ch == NULL){
        return STATUS_ERROR_NULL;
    }

    /* Check if UART is initialized */
    if(uart_initialized == 0U){
        return STATUS_ERROR_INIT;
    }

    /* check if data is available */
    if(UART_IsCharAvailable() == 0U){
        return STATUS_PENDING;
    }

    /* Read the received character (clears RXNE) */
    *ch = (char)(USART2->DR & 0xFFU);

    return STATUS_OK;
}