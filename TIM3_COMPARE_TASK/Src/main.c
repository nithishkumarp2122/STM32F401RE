#include "stm32f401xe.h"

#define GPIOAEN (1<<0)
#define TIM3EN  (1<<1)
#define CC1E    (1<<0)
#define CEN     (1<<0)

int main()
{
    /* 1. Enable GPIOA clock */
    RCC->AHB1ENR |= GPIOAEN;

    /* 2. Set PA6 to Alternate Function mode */
    GPIOA->MODER &= ~(3<<12);
    GPIOA->MODER |=  (2<<12);

    /* 3. Select AF2 for TIM3_CH1 */
    GPIOA->AFR[0] &= ~(0xF<<24);
    GPIOA->AFR[0] |=  (2<<24);

    /* 4. Enable TIM3 clock */
    RCC->APB1ENR |= TIM3EN;

    /* 5. Timer configuration */
    TIM3->PSC = 16000 - 1;   // Prescaler
    TIM3->ARR = 1000 - 1;    // Auto reload
    TIM3->CNT = 0;
    /* 6. Output Compare Toggle Mode */
    TIM3->CCMR1 &= ~(7<<4);
    TIM3->CCMR1 |=  (3<<4);   // Toggle mode

    /* 7. Enable Channel 1 output */
    TIM3->CCER |= CC1E;

    /* 8. Compare value */
    TIM3->CCR1 = 500;

    /* 9. Start timer */
    TIM3->CR1 |= CEN;

    while(1)
    {

    }
}
