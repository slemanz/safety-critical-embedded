#include <stdint.h>
#include <stdio.h>
#include "stm32f411xe.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"

int main(void)
{
    fpu_enable();
    debug_uart_init();
    timebase_init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~(3U << (13*2));
    GPIOC->MODER |=  (1U << (13*2));

    printf("Hello world!\r\n");
    printf("Debug in on!\r\n");

    while(1)
    {
        GPIOC->ODR ^= (1U << 13);
        tick_delay(1);
    }
}