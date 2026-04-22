#include "stm32f401xe.h"
#include <stdint.h>

#define USART1_BAUDRATE 9600
#define SYS_CLOCK 16000000

void USART1_RX_INIT(void)
{
    /* Enable clocks */
    RCC->AHB1ENR |= (1 << 0);     // GPIOA clock
    RCC->APB2ENR |= (1 << 4);     // USART1 clock

    GPIOA->MODER &= ~(3 << 20);
    GPIOA->MODER |=  (2 << 20);   // AF mode

    /* Set AF7 (USART1_RX) */
    GPIOA->AFR[1] &= ~(0xF << 8);
    GPIOA->AFR[1] |=  (7 << 8);

    /* Configure baud rate */
    USART1->BRR = SYS_CLOCK / USART1_BAUDRATE;

    /* Enable Receiver */
    USART1->CR1 |= (1 << 2);      // RE

    /* Enable USART */
    USART1->CR1 |= (1 << 13);     // UE
}

char USART1_read(void)
{
    while(!(USART1->SR & (1 << 5)));  // Wait until RXNE is set
    return (char)USART1->DR;
}

int main(void)
{
    USART1_RX_INIT();

    while(1)
    {
        char receive = USART1_read();
    }
}
