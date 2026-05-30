#include "stm32f411xe.h"

#define GPIOAEN         (1U << 0)
#define UART2EN         (1U << 17)

#define DBG_UART_BAUDRATE           (115200)
#define SYS_FREQ                    (16000000)
#define APB1_FREQ                   SYS_FREQ

#define CR1_TE                      (1U << 3)
#define CR1_UE                      (1U << 13)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);

void debug_uart_init(void)
{
    /* Enable clock access to GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Set the mode of PA2 to alternate function mode */
    GPIOA->MODER &= ~(1U << 4);
    GPIOA->MODER |=  (1U << 5);

    /* Set alternate funtion type to AF7 (UART2_TX) */
    GPIOA->AFR[0] |= (1U << 8);
    GPIOA->AFR[0] |= (1U << 9);
    GPIOA->AFR[0] |= (1U << 10);
    GPIOA->AFR[0] &= ~(1U << 11);

    /* Enable clock access to UART2 */
    RCC->APB1ENR |= UART2EN;

    /* Configure uart baudrate */
    compute_uart_bd(APB1_FREQ, DBG_UART_BAUDRATE);

    /* Configure transfer direction */
    USART2->CR1 = CR1_TE;

    /* Enable UART Module */
    USART2->CR1 |= CR1_UE;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
    return ((periph_clk + (baudrate/2U))/baudrate);
}