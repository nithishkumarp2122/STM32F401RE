#include "stm32f401xe.h"
#include "stdint.h"

#define GPIOAEN (0X1UL << 0U)


#define TIM2EN (0X1UL << 0U)
#define OC1E_0001 (0X1UL << 0U)
#define CEN_0001  (0X1UL << 0U)

void COMPARE_INIT();
void TIMER2_OUTPUT_COMPARE();
void SET();
void RESET();

int main()
{
	COMPARE_INIT();
    while (1){
    	TIMER2_OUTPUT_COMPARE();
    }
}

void TIMER2_OUTPUT_COMPARE()
{

    SET();

    // Delay is observation of high state (e.g., for testing)
    for (volatile int i = 0; i < 10000; i++);

    RESET();
    for (volatile int i = 0; i < 10000; i++);
}

void COMPARE_INIT(){

	    RCC->AHB1ENR |= GPIOAEN;

	    GPIOA->MODER |= (0x1<<31);

	    GPIOA->AFR[1] |= (0x1<<28);

	    RCC->APB1ENR |= TIM2EN;

	    TIM2->PSC = 16000 - 1;
	    TIM2->ARR = 2000;
	    TIM2->CNT = 0;
	    TIM2->CCER |= OC1E_0001;
	    TIM2->CR1 |= CEN_0001;
}

void SET() {
    // Output high on match (OC1REF => high when TIMx_CNT = TIMx_CCR1)
    TIM2->CCMR1 |= (0x1<<4U);

    TIM2->CCR1 = 1000;

    while(!(TIM2->SR & TIM_SR_UIF));

    TIM2->SR &= ~TIM_SR_UIF;
}

void RESET() {
    // Output low on match (OC1REF => when TIMx_CNT = TIMx_CCR1)
    TIM2->CCMR1 |= (0x1<<5U);

    TIM2->CCR1 = 1500;

    while(!(TIM2->SR & TIM_SR_UIF));

    TIM2->SR &= ~TIM_SR_UIF;
}
