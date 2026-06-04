#include <stdint.h>
#include <stdio.h>
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"

int main(void)
{
    fpu_enable();
    debug_uart_init();
    timebase_init();
    led_init();
    button_init();
    pa1_adc_init();
    start_conversion();

    printf("Hello world!\r\n");
    printf("Debug in on!\r\n");

    uint32_t last_tick = tick_get();

    while(1)
    {
        if(button_get_state())
        {
            led_on();
        }
        else
        {
            led_off();
        }

        if((tick_get() - last_tick) >= 5)
        {
            last_tick = tick_get();
            printf("ADC: %lu\r\n", adc_read());
        }
    }
}