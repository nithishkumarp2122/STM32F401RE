#include"stm32f401xe.h"
#include"stdint.h"

void TIMER2_PWM();
void TIMER2_PWM_DECREMENT();
void TIMER2_PWM_INCREMENT();

int main()
{
	TIMER2_PWM();
	while(1)
	{
		TIMER2_PWM_INCREMENT();
		for(int i=0;i<1000000;i++);
		TIMER2_PWM_DECREMENT();
		for(int i=0;i<1000000;i++);
	}
}

void TIMER2_PWM()
{
	RCC->AHB1ENR |= (1<<0);

	GPIOA->MODER |= (1<<31);

	GPIOA->AFR[1] |= (1<<28);

	RCC->APB1ENR |= (1<<0);

	TIM2->PSC = 16000 - 1 ;

	TIM2->ARR = 100;

	TIM2->CNT = 0 ;

	TIM2->CCMR1 |= (0X6UL << 4U);

	TIM2->CCER |= (1<<0);

	TIM2->CR1 = (1<<0);
}


void TIMER2_PWM_INCREMENT()
{
	int i;
	TIM2->CCR1 = 0;  // Initialize duty cycle to 0.
	for(i=0;i<=100;i+=10)
	{
		TIM2->CCR1 = i;
		while(!(TIM2->SR & TIM_SR_UIF)); // Wait for update interrupt flag.
		TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag.
	}
}

void TIMER2_PWM_DECREMENT()
{
	int i;
	TIM2->CCR1 = 100; // Initialize duty cycle to 100.
	for(i=100;i>=0;i-=10)
	{
		TIM2->CCR1 = i;
		while(!(TIM2->SR & TIM_SR_UIF)); // Wait for update interrupt flag.
		TIM2->SR &= ~TIM_SR_UIF; //Clear the update interrupt flag.
	}
}
