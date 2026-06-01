#include "bsp.h"
#include "stm32f411xe.h"

#define PIN13           (1U << 13)
#define LED_PIN         PIN13
#define PIN0            (1U << 0)
#define BTN_PIN         PIN0

void led_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~(3U << (13*2));
    GPIOC->MODER |=  (1U << (13*2));
}

void led_on(void)
{
    GPIOC->ODR &= ~(LED_PIN);
}

void led_off(void)
{
    GPIOC->ODR |= (LED_PIN);
}

void led_toggle(void)
{
    GPIOC->ODR ^= (LED_PIN);
}

void button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA0 as input pull up */
    GPIOA->MODER &= ~(3U << (0*2));

    GPIOA->PUPDR &= ~(3U << (0*2));
    GPIOA->PUPDR |=  (1U << (0*2));
}

bool button_get_state(void)
{
    if(GPIOA->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}
}