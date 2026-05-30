#include <stdint.h>
#include "stm32f411xe.h"
#include "fpu.h"
#include "uart.h"

int main(void)
{
    fpu_enable();
    debug_uart_init();
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~(3U << (13*2));
    GPIOC->MODER |=  (1U << (13*2));

    while(1)
    {
        GPIOC->ODR ^= (1U << 13);
        for (volatile uint32_t i = 0; i < 1000000; i++);
    }
}