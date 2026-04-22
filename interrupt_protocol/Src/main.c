#include "stm32f401xe.h"
#include "stdint.h"

#define GPIOAEN (1U << 0)
#define USART2EN (1U<<17)

#define CR1_TE (1U << 3)
#define CR1_RE (1U << 2)
#define CR1_UE (1U << 13)
#define SR_TXE (1U << 7)
#define SR_RXNE (1U << 5)
#define CR1_RXNEIE (1U<<5)

#define APB1CLK 16000000
#define USART_BAUDRATE 9600

void USART2_TX_RX_INTERRUPT()
{
	    RCC->AHB1ENR |= GPIOAEN;
	    RCC->APB1ENR |= USART2EN;

	    GPIOA->MODER &=~(1U<<4);
	    GPIOA->MODER |= (1U<<5);

	    GPIOA->AFR[0] |=(1U<<8); //0111: AF7
	    GPIOA->AFR[0] |=(1U<<9);
	    GPIOA->AFR[0] |=(1U<<10);
	    GPIOA->AFR[0] &=~(1U<<11);

	    GPIOA->MODER &=~(1U<<6);
	    GPIOA->MODER |= (1U<<7);

	    GPIOA->AFR[0] |=(1U<<12);
	    GPIOA->AFR[0] |=(1U<<13);
	    GPIOA->AFR[0] |=(1U<<14);
	    GPIOA->AFR[0] &=~(1U<<15);

	    USART2->BRR = APB1CLK/USART_BAUDRATE;

	    USART2->CR1 |= CR1_TE;
	    USART2->CR1 |= CR1_RE;

	    USART2->CR1 |= CR1_RXNEIE;

	    USART2->CR1 |= CR1_UE;

	    NVIC_EnableIRQ(USART2_IRQn);

}

void USART2_WRITE(char dat)
{
    while (!(USART2->SR & SR_TXE));
    USART2->DR = dat;
}

char USART2_READ()
{
    while (!(USART2->SR & SR_RXNE));
    return USART2->DR;
}

// SET PRIORITY.
// NVIC_SetPriority(USART2_IRQn, 1);

void USART2_IRQHandler()
{
    if (USART2->SR & SR_RXNE)
    {
        char received = USART2_READ();
        USART2_WRITE(received);
        GPIOA->ODR ^= (0x1<<5);
    }
}


int main()
{
    RCC->AHB1ENR |= GPIOAEN;
    GPIOA->MODER |= (1U << 10);//led output mode

    USART2_TX_RX_INTERRUPT();

    USART2_WRITE('A');
    for (int i = 0; i < 100000; ++i);

    while (1)
    {

    } // while(1);
}
