#include "stm32f401xe.h"
#include "stdint.h"

void TIM5_COUNTER();
void TIM5_COUNT_INIT();

int main()
{
	RCC->AHB1ENR |= 0X1<<0;
	GPIOA->MODER |= 0X1<<10;
	GPIOA->MODER |= 0X1<<12;

	TIM5_COUNT_INIT();
	while(1)
	{
		GPIOA->ODR |= (1<<5) ;
		GPIOA->ODR |= (1<<6);
		TIM5_COUNTER();

		GPIOA->ODR &=~ (1<<5);
		GPIOA->ODR &=~ (1<<6);
		TIM5_COUNTER();
	}
}

void TIM5_COUNT_INIT()
{
	RCC->APB1ENR  |= RCC_APB1ENR_TIM5EN;
	GPIOA->MODER  |= 0X1<<3;
	GPIOA->AFR[0] |= 0X2<<4;

	TIM5->ARR = 2;
	TIM5->CNT = 1;
	TIM5->CCMR1 |= 0X1<<8;
	TIM5->CCER  |= 0X1<<5;
	TIM5->SMCR  |= 0X3<<5;
	TIM5->SMCR  |= 0X7<<0;
	TIM5->CR1   |=  TIM_CR1_CEN;
}

void TIM5_COUNTER()
{
	while(!(TIM5->SR & TIM_SR_UIF));
	TIM5->SR &= ~TIM_SR_UIF;
}
