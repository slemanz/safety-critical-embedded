#include "timebase.h"
#include "stm32f411xe.h"

#define CTRL_ENABLE         (1U << 0)
#define CTRL_TICKINT        (1U << 1)
#define CTRL_CLCKSRC        (1U << 2)
#define CTRL_COUNTFLAG      (1U << 16)

/* 1 ms tick at 16 MHz HSI -> 16000 cycles per tick */
#define ONE_MS_LOAD         16000U

#define MAX_DELAY           0xFFFFFFFFU
#define TICK_FREQ           1U

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;

void tick_increment(void)
{
    g_curr_tick += TICK_FREQ;
}

uint32_t tick_get(void)
{
    __disable_irq();
    g_curr_tick_p = g_curr_tick;
    __enable_irq();
    return g_curr_tick_p;
}

void tick_delay(uint32_t delay)
{
    uint32_t tickstart = tick_get();
    uint32_t wait = delay;

    if(wait < MAX_DELAY)
    {
        wait += (uint32_t)TICK_FREQ;
    }

    while((tick_get() - tickstart) < wait){}
}

void timebase_init(void)
{
    /* disable global interrupts */
    __disable_irq();

    /* load the timer with number of clock cycles per millisecond */
    SysTick->LOAD = ONE_MS_LOAD - 1U;

    /* clear systick current value register */
    SysTick->VAL = 0;

    /* select internal clock source */
    SysTick->CTRL = CTRL_CLCKSRC;

    /* enable interrupt */
    SysTick->CTRL |= CTRL_TICKINT;

    /* enable systick */
    SysTick->CTRL |= CTRL_ENABLE;

    /* enable global interrupt */
    __enable_irq();
}

void SysTick_Handler(void)
{
	tick_increment();
}