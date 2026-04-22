#include<stm32f401xe.h>
#define USART1_BAUDRATE 9600
void USART1_TX_INIT();
void USART1_write(char a);
void STRING_TX(char *ptr);


int main()
{
	USART1_TX_INIT();
	STRING_TX("NITHISH");

	while(1)
	{

		for(volatile int i = 0; i < 100000; i++);
	}
}

void USART1_TX_INIT()
{
	 // Enable clocks
	    RCC->AHB1ENR |= (1<<0);    // GPIOA clock
	    RCC->APB2ENR |= (1<<4);    // USART1 clock

	    // PA9 → Alternate Function mode
	    GPIOA->MODER &= ~(3 << 18);
	    GPIOA->MODER |=  (2 << 18);

	    // PA9 → AF7 (USART1_TX)
	    GPIOA->AFR[1] |= (7 << 4);   // PA9 is AFR[1], bits [7:4]

	    // Baud rate (APB2 = 16 MHz)
	    USART1->BRR = 16000000 / USART1_BAUDRATE;

	    // Enable transmitter and USART
	    USART1->CR1 |= (1<<3);    // TE
	    USART1->CR1 |= (1<<13);   // UE
}

void USART1_write(char a)
{
	USART1->DR = a;
	while(!(USART1->SR & USART_SR_TXE));

}
void STRING_TX(char *ptr)
{
	while(*ptr)
	{
		USART1_write(*ptr++);
	}
}
