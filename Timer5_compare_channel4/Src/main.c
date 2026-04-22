#include "stm32f401xe.h"
#include "stdint.h"

#define GPIOAEN (1<<0)
#define TIM5EN  (1<<3)

#define OC4E_0001 (0X1UL << 12U)
#define CEN_0001  (0X1UL << 0U)

void COMPARE_INIT();
void TIMER5_OUTPUT_COMPARE();
void SET();
void RESET();

int main()
{
    COMPARE_INIT();
    while (1)
    {
        TIMER5_OUTPUT_COMPARE();
    }
}

void TIMER5_OUTPUT_COMPARE()
{
    SET();

    for (volatile int i = 0; i < 10000; i++);

    RESET();

    for (volatile int i = 0; i < 10000; i++);
}

void COMPARE_INIT(){

    RCC->AHB1ENR |= GPIOAEN;

    /* PA3 -> TIM5_CH4 */
    GPIOA->MODER |= (1<<5);
    GPIOA->AFR[0] |= (2<<12);

    RCC->APB1ENR |= TIM5EN;

    TIM5->PSC = 16000 - 1;
    TIM5->ARR = 2000;
    TIM5->CNT = 0;
    TIM5->CCER |= OC4E_0001;
    TIM5->CR1 |= CEN_0001;
}

void SET() {

    TIM5->CCMR2 |= (1<<14);
    TIM5->CCR4 = 1000;

    while(!(TIM5->SR & TIM_SR_UIF));

    TIM5->SR &= ~TIM_SR_UIF;
}

void RESET() {

    TIM5->CCMR2 |= (1<<13);
    TIM5->CCR4 = 1500;

    while(!(TIM5->SR & TIM_SR_UIF));

    TIM5->SR &= ~TIM_SR_UIF;
}
