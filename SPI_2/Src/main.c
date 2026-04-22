#include "stm32f401xe.h"

void GPIO_init(void)
{
    // Enable clock for GPIOB
    RCC->AHB1ENR |= (1<<1);

    // PB13 (SCK), PB14 (MISO), PB15 (MOSI) → Alternate Function
    GPIOB->MODER &= ~(0x3F << 26);
    GPIOB->MODER |=  (0x2A << 26);

    // Set AF5 (SPI2)
    GPIOB->AFR[1] |= (5<<20); // PB13
    GPIOB->AFR[1] |= (5<<24); // PB14
    GPIOB->AFR[1] |= (5<<28); // PB15

    // PB12 → NSS as GPIO Output
    GPIOB->MODER &= ~(3<<24);
    GPIOB->MODER |=  (1<<24);

    // Set NSS HIGH initially
    GPIOB->ODR |= (1<<12);
}

void SPI2_init(void)
{
    GPIO_init();

    // Enable SPI2 clock (APB1)
    RCC->APB1ENR |= (1<<14);

    // Master mode
    SPI2->CR1 |= (1<<2);

    // Baud rate = fPCLK / 16
    SPI2->CR1 |= (3<<3);

    // Software slave management (SSM=1, SSI=1)
    SPI2->CR1 |= (1<<9) | (1<<8);

    // Enable SPI
    SPI2->CR1 |= (1<<6);
}

void SPI2_write(char data)
{
    while(!(SPI2->SR & SPI_SR_TXE)); // wait buffer empty
    SPI2->DR = data;

    while(SPI2->SR & SPI_SR_BSY);    // wait till done
}

int main(void)
{
    SPI2_init();

    // Start communication (NSS LOW)
    GPIOB->ODR &= ~(1<<12);

    // Send 2 characters
    SPI2_write('A');
    SPI2_write('B');

    // End communication (NSS HIGH)
    GPIOB->ODR |= (1<<12);

    while(1);
}
