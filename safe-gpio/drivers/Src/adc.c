#include "adc.h"
#include "stm32f411xe.h"

void pa1_adc_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (0x3U << 2);

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->SQR3 = (1 << 0);

    ADC1->CR2 |= ADC_CR2_ADON;
}

void start_conversion(void)
{
    ADC1->CR2 |= ADC_CR2_CONT;
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adc_read(void)
{
    while(!(ADC1->SR & ADC_SR_EOC)){}

    return (ADC1->DR);
}