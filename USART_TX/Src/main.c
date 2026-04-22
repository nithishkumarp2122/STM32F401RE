#include "stm32f401xe.h"
#include "stdint.h"

#define GPIOAEN (1<<0)
#define USART2EN (1<< 17)

#define CR1_TE (1<<3)
#define CR1_UE (1<<13)
#define SR_TXE (1<<7)

#define SYS_FREQ 16000000
#define USART_BAUDRATE 9600

void USART2_TX_RX_INIT()
{
    RCC->AHB1ENR |= GPIOAEN;
    RCC->APB1ENR |= USART2EN;

    GPIOA->MODER &=~(1U<<4);
    GPIOA->MODER |= (1U<<5);


    GPIOA->AFR[0] |=(1<<8);
    GPIOA->AFR[0] |=(1<<9);
    GPIOA->AFR[0] |=(1<<10);
    GPIOA->AFR[0] &=~(1<<11);


    USART2->BRR = SYS_FREQ/USART_BAUDRATE;
    USART2->CR1 |= CR1_TE; //Transmitter
    USART2->CR1 |= CR1_UE;
}



int main()
{
    USART2_TX_RX_INIT();

    while (1)
    {

        USART2->DR = 'r';
        while (!(USART2->SR & SR_TXE));

        for (int i = 0; i < 100000; ++i);


    }
}
