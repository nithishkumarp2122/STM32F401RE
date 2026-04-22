#include "stm32f401xe.h"
#include "stdint.h"

/* ---------------- CLOCK ENABLE ---------------- */
#define GPIOAEN (1U<<0)
#define GPIOBEN (1U<<1)
#define SPI2EN  (1U<<14)

/* ---------------- LCD CONTROL PINS (PB0,1,2) ---------------- */
#define RS (1U<<0)
#define RW (1U<<1)
#define EN (1U<<2)

/* ---------------- SPI STATUS FLAGS ---------------- */
#define TXE  (1U<<1)
#define RXNE (1U<<0)
#define BUSY (1U<<7)

/* ---------------- FUNCTION DECLARATIONS ---------------- */
void GPIO_INIT(void);
void SPI2_INIT(void);
void SPI_MO(uint8_t data);
uint8_t SPI_MI(void);

void SPI_BYTEWRITE(void);
void SPI_BYTEREAD(char *buffer);

void lcd_init(void);
void send_cmd(unsigned char cmd);
void send_data(unsigned char data);
void show_data(char *s);
void delay(int ms);

/* ---------------- GPIO INIT ---------------- */
void GPIO_INIT()
{
    RCC->AHB1ENR |= GPIOAEN | GPIOBEN;

    /* PA0–PA7 -> LCD DATA */
    GPIOA->MODER |= 0x5555;

    /* PB13 -> SCK, PB15 -> MOSI */
    GPIOB->MODER &= ~(3U<<26);
    GPIOB->MODER |=  (2U<<26);

    GPIOB->MODER &= ~(3U<<30);
    GPIOB->MODER |=  (2U<<30);

    GPIOB->AFR[1] |= (5U<<20); // PB13
    GPIOB->AFR[1] |= (5U<<28); // PB15

    /* PB9 -> CS */
    GPIOB->MODER &= ~(3U<<18);
    GPIOB->MODER |=  (1U<<18);

    /* PB0,1,2 -> LCD control */
    GPIOB->MODER |= (1U<<0) | (1U<<2) | (1U<<4);
}

/* ---------------- SPI INIT ---------------- */
void SPI2_INIT()
{
    RCC->APB1ENR |= SPI2EN;

    SPI2->CR1 = 0;
    SPI2->CR1 |= (1<<2);   // Master
    SPI2->CR1 |= (1<<9);   // SSM
    SPI2->CR1 |= (1<<8);   // SSI
    SPI2->CR1 |= (3<<3);   // Baud rate
    SPI2->CR1 |= (1<<6);   // Enable
}

/* ---------------- SPI WRITE ---------------- */
void SPI_MO(uint8_t data)
{
    while (!(SPI2->SR & TXE));
    SPI2->DR = data;
    while (SPI2->SR & BUSY);
}

/* ---------------- SPI READ ---------------- */
uint8_t SPI_MI()
{
    while (!(SPI2->SR & TXE));
    SPI2->DR = 0xFF;

    while (!(SPI2->SR & RXNE));
    return SPI2->DR;
}

/* ---------------- EEPROM WRITE ---------------- */
void SPI_BYTEWRITE()
{
    char *data = "NITHISH";

    /* WRITE ENABLE */
    GPIOB->ODR &= ~(1<<9);
    SPI_MO(0x06);
    GPIOB->ODR |= (1<<9);

    delay(1);

    /* WRITE DATA */
    GPIOB->ODR &= ~(1<<9);
    SPI_MO(0x02);
    SPI_MO(0x00);
    SPI_MO(0x00);

    for(int i=0; data[i] != '\0'; i++)
    {
        SPI_MO(data[i]);
    }

    GPIOB->ODR |= (1<<9);

    delay(10); // EEPROM write time
}

/* ---------------- EEPROM READ ---------------- */
void SPI_BYTEREAD(char *buffer)
{
    GPIOB->ODR &= ~(1<<9);

    SPI_MO(0x03);
    SPI_MO(0x00);
    SPI_MO(0x00);

    for(int i=0; i<7; i++)
    {
        buffer[i] = SPI_MI();
    }
    buffer[7] = '\0';

    GPIOB->ODR |= (1<<9);
}

/* ---------------- LCD INIT ---------------- */
void lcd_init()
{
    delay(20);
    send_cmd(0x38);
    send_cmd(0x0E);
    send_cmd(0x01);
    send_cmd(0x06);
}

/* ---------------- LCD COMMAND ---------------- */
void send_cmd(unsigned char cmd)
{
    GPIOA->ODR = cmd;
    GPIOB->ODR &= ~RS;
    GPIOB->ODR &= ~RW;
    GPIOB->ODR |= EN;
    delay(2);
    GPIOB->ODR &= ~EN;
}

/* ---------------- LCD DATA ---------------- */
void send_data(unsigned char data)
{
    GPIOA->ODR = data;
    GPIOB->ODR |= RS;
    GPIOB->ODR &= ~RW;
    GPIOB->ODR |= EN;
    delay(2);
    GPIOB->ODR &= ~EN;
}

/* ---------------- LCD STRING ---------------- */
void show_data(char *s)
{
    while(*s)
    {
        send_data(*s++);
    }
}

/* ---------------- DELAY ---------------- */
void delay(int ms)
{
    for(int i=0;i<ms;i++)
        for(int j=0;j<16000;j++);
}

/* ---------------- MAIN ---------------- */
int main()
{
    char buffer[8];

    GPIO_INIT();
    SPI2_INIT();
    lcd_init();

    SPI_BYTEWRITE();
    SPI_BYTEREAD(buffer);

    while (1)
    {
        send_cmd(0x80);
        show_data(buffer);
    }
}
