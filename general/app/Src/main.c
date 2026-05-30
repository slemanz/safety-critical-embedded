#include <stdint.h>
#include <stdio.h>
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"

int main(void)
{
    fpu_enable();
    debug_uart_init();
    timebase_init();
    led_init();

    printf("Hello world!\r\n");
    printf("Debug in on!\r\n");


    while(1)
    {
        led_toggle();
        tick_delay(1);
    }
}