#include "stm32f401xe.h"
#include <stdint.h>

#define GPIOAEN (1U << 0)
#define ADC1EN  (1U << 8)

#define ADON    (1U << 0)
#define SWSTART (1U << 30)
#define SR_EOC  (1U << 1)

void ADC_INIT(void);
uint32_t ADC_READ(void);
void LED_GPIO(void);

int main()
{
    LED_GPIO();
    ADC_INIT();

    while (1)
    {
        uint32_t sensor = ADC_READ();

        if (sensor > 2000)
        {
            GPIOA->ODR |= (1 << 5);   // LED ON
        }
        else
        {
            GPIOA->ODR &= ~(1 << 5);  // LED OFF
        }
    }
}

void ADC_INIT(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= GPIOAEN;

    // PA2 → Analog mode (Channel 2)
    GPIOA->MODER &= ~(3U << 4);
    GPIOA->MODER |=  (3U << 4);

    // Enable ADC clock
    RCC->APB2ENR |= ADC1EN;

    // Optional: set sampling time (IMPORTANT for stability)
    ADC1->SMPR2 |= (7U << 6);   // Channel 2 → max sample time

    // Select channel 2
    ADC1->SQR3 = 2;
    // 1 conversion
    ADC1->SQR1 = 0;

    // Enable ADC
    ADC1->CR2 |= ADON;
}

uint32_t ADC_READ(void)
{
    // Start conversion
    ADC1->CR2 |= SWSTART;

    // Wait for conversion complete
    while (!(ADC1->SR & SR_EOC));

    return ADC1->DR;
}

void LED_GPIO(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= GPIOAEN;

    // PA5 → Output mode
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);
}
