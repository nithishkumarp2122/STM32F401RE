#include "stm32f401xe.h"

#define GPIOAEN (1<<0)
#define TIM3EN  (1<<1)

int main()
{

    RCC->AHB1ENR |= GPIOAEN;
    RCC->APB1ENR |= TIM3EN;


    GPIOA->MODER |= (2<<14);      // PA7 alternate function mode
    GPIOA->AFR[0] |= (2<<28);     // AF2 = TIM3


    TIM3->PSC = 16000-1;          // 1 ms timer tick
    TIM3->ARR = 1000-1;           // 1 second period
    TIM3->CCR2 = 500;             // Compare value

    /* Output Compare Toggle Mode for Channel 2 */
    TIM3->CCMR1 |= (3<<12);       // OC2M = Toggle mode
    /* Enable Channel 2 */
    TIM3->CCER |= (1<<4);         // CC2E enable
    /* Start Timer */
    TIM3->CR1 |= (1<<0);

    while(1);
}
