#include "stm32f401xe.h"
#include <stdio.h>

// LCD control pins
#define RS (1<<0)
#define RW (1<<1)
#define EN (1<<2)

// Buttons
#define BTN1 (1<<8)
#define BTN2 (1<<9)

void delay(int ms);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_print(char *s);
void lcd_init();

//---------------- LCD ----------------//
void lcd_cmd(unsigned char cmd)
{
    GPIOB->ODR = cmd;          // Data
    GPIOA->ODR &= ~RS;
    GPIOA->ODR &= ~RW;
    GPIOA->ODR |= EN;
    delay(2);
    GPIOA->ODR &= ~EN;
}

void lcd_data(unsigned char data)
{
    GPIOB->ODR = data;
    GPIOA->ODR |= RS;
    GPIOA->ODR &= ~RW;
    GPIOA->ODR |= EN;
    delay(2);
    GPIOA->ODR &= ~EN;
}

void lcd_print(char *s)
{
    while(*s) lcd_data(*s++);
}

void lcd_init()
{
    delay(20);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x06);
}

//---------------- DELAY ----------------//
void delay(int ms)
{
    for(int i=0;i<ms;i++)
        for(int j=0;j<1000;j++);
}

//---------------- MAIN ----------------//
int main()
{
    RCC->AHB1ENR |= (1<<0); // GPIOA
    RCC->AHB1ENR |= (1<<1); // GPIOB

    // PORTB → LCD DATA
    GPIOB->MODER = 0x55555555;

    // PA0,1,2 → output
    GPIOA->MODER |= (1<<0)|(1<<2)|(1<<4);

    // PA8,9 → input
    GPIOA->MODER &= ~(3<<16);
    GPIOA->MODER &= ~(3<<18);

    // pull-down
    GPIOA->PUPDR |= (2<<16);
    GPIOA->PUPDR |= (2<<18);

    lcd_init();

    uint32_t odd = 0, even = 0;
    char buf[16];

    while(1)
    {
        // BTN1 → TIM2 (ODD)
        if(GPIOA->IDR & BTN1)
        {
            odd++;
            delay(200); // debounce
        }

        // BTN2 → TIM5 (EVEN)
        if(GPIOA->IDR & BTN2)
        {
            even++;
            delay(200);
        }

        // Display
        lcd_cmd(0x80);
        sprintf(buf,"ODD:%lu ",odd);
        lcd_print(buf);

        lcd_cmd(0xC0);
        sprintf(buf,"EVEN:%lu",even);
        lcd_print(buf);
    }
}
