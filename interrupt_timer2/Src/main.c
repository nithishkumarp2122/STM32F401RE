#include "stm32f401xe.h"
#include "stdint.h"

#define DIER_UIE (1U<<0)
#define UIF (1U<<0)

void TIM2_DELAY(uint32_t ms);
void TIM2_1Hz_INTERRUPT();
static void Timer2_callback();


int main()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= 0X1UL<<10U;
    TIM2_1Hz_INTERRUPT();

    while (1)
    {
        // Main loop waiting for interrupts.
    }
}

void TIM2_1Hz_INTERRUPT()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 16000 - 1;
    TIM2->ARR = 1000 - 1;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
    // Enable Timer interrupt.
    TIM2->DIER |= DIER_UIE;

    // Enable Timer2 interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn); // This function allows you to enable specific interrupt requests (IRQs) for peripherals,
                               //  enabling the associated interrupt service routines (ISRs) to be executed when the interrupt condition occurs.
}

// Timer2 interrupt handler
void TIM2_IRQHandler()
{
    // Clear the update interrupt flag
    TIM2->SR &= ~UIF; //After processing an interrupt, clearing the flag allows the timer to continue counting without any error.
   // TIM2->SR = UIF; //Incorrect method.
    GPIOA->ODR ^= (0X1<<5);
}
