#include "stm32f411xe.h"

#define GPIOAEN         (1U << 0)
#define UART2EN         (1U << 17)

#define DBG_UART_BAUDRATE           (115200)
#define SYS_FREQ                    (16000000)
#define APB1_FREQ                   SYS_FREQ

#define CR1_TE                      (1U << 3)
#define CR1_UE                      (1U << 13)
#define SR_TXE                      (1U << 7)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);

int __io_putchar(int ch)
{
    uart_write(ch);
    return ch;
}

void debug_uart_init(void)
{
    /* Enable clock access to GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Set alternate funtion type to AF7 (UART2_TX) */
    GPIOA->AFR[0] |= (1U << 8);
    GPIOA->AFR[0] |= (1U << 9);
    GPIOA->AFR[0] |= (1U << 10);
    GPIOA->AFR[0] &= ~(1U << 11);

    /* Set the mode of PA2 to alternate function mode */
    GPIOA->MODER &= ~(1U << 4);
    GPIOA->MODER |=  (1U << 5);

    /* Enable clock access to UART2 */
    RCC->APB1ENR |= UART2EN;

    /* Configure uart baudrate */
    USART2->BRR = compute_uart_bd(APB1_FREQ, DBG_UART_BAUDRATE);

    /* Configure transfer direction */
    USART2->CR1 = CR1_TE;

    /* Enable UART Module */
    USART2->CR1 |= CR1_UE;
}

static void uart_write(int ch)
{
    /* Make sure transmit data register is empty */
    while(!(USART2->SR & SR_TXE)){}

    /* Write to transmit data register */
    USART2->DR = (ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
    return ((periph_clk + (baudrate/2U))/baudrate);
}