#include "stm32f401xe.h"
#include "stdint.h"

void TIM2_COUNTER();
void TIM2_COUNT_INIT();

int main()
{
	RCC->AHB1ENR |= 0X1UL<<0U ;

	GPIOA->MODER |= 0X1UL<<10U;

	GPIOA->MODER |= 0X1UL<<12U;

	TIM2_COUNT_INIT();
	while(1)
	{
		GPIOA->ODR = 0x60
		TIM2_COUNTER();

		GPIOA->ODR = 0x00
		TIM2_COUNTER();
	}
}

void TIM2_COUNT_INIT()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	GPIOA->MODER |= 0X1UL<<3U;

	GPIOA->AFR[0] |= 0X1UL<<4U;

	TIM2->ARR = 5;

	TIM2->CNT = 1;

	TIM2->CCMR1 |= 0X1<<8;

	TIM2->CCER |= (0X1<<5);

	TIM2->SMCR |= 0X3<<5;

	TIM2->SMCR |= 0X7<<0;

	TIM2->CR1 |=  TIM_CR1_CEN;
}

void TIM2_COUNTER()
{
	while(!(TIM2->SR & TIM_SR_UIF));
	TIM2->SR &= ~TIM_SR_UIF;
}
