#include <stdint.h>
#include "stm32f411xe.h"
#include "safe_status.h"
#include "gpio_demo.h"
#include "timebase.h"

static void system_init(void)
{
    /* configure timebase (1 ms tick @ 16 MHz HSI) */
    timebase_init();

    /* Enable all fault handlers */

    /* Enable global interrupts */
}


int main(void)
{
    Status_t status;

    system_init();

    /* Allow system to stabilize */
    tick_delay(100U);

    status = GPIO_Demo_Init();
    if(status != STATUS_OK){
        /* Demo init failed: halt safely */
        while(1){}
    }
    tick_delay(1000U);

    while(1)
    {
        (void)GPIO_Demo_Run();
    }
}
