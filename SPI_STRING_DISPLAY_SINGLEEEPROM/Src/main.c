#include "stm32f401xe.h"
#include "stdint.h"

// Bit Definitions
#define RXNE    (1U << 0)
#define TXE     (1U << 1)
#define BUSY    (1U << 7)

// LCD Pin Definitions (Assuming GPIOC for Data, GPIOB for Control)
#define RS (1U << 0)  // PB0
#define RW (1U << 1)  // PB1
#define EN (1U << 2)  // PB2

// Prototypes
void GPIO_Init(void);
void SPI1_Master_Init(void);
void SPI_Transmit(uint8_t data);
uint8_t SPI_Receive(void);
void lcd_init(void);
void send_cmd(unsigned char cmd);
void send_data(unsigned char data);
void show_data(char *s);
void delay(int ms);

void GPIO_Init() {
    // Enable GPIO Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

    // --- SPI1 Pins (PA5=CLK, PA6=MISO, PA7=MOSI) ---
    GPIOA->MODER |= (2U << 10) | (2U << 12) | (2U << 14); // Alternate Function
    GPIOA->AFR[0] |= (5U << 20) | (5U << 24) | (5U << 28); // AF5 for SPI1

    // SPI Chip Select (PA4) - Manual Output
    GPIOA->MODER |= (1U << 8);
    GPIOA->ODR   |= (1U << 4); // Default High

    // --- LCD Pins ---
    // Data Lines: PC0-PC7
    GPIOC->MODER |= 0x5555;
    // Control Lines: PB0, PB1, PB2
    GPIOB->MODER |= (1U << 0) | (1U << 2) | (1U << 4);
}

void SPI1_Master_Init() {
    GPIO_Init();
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1 Clock

    // CR1 settings: Baud rate fPCLK/16, MSTR mode, CPOL/CPHA = 0
    SPI1->CR1 = (3U << 3) | (1U << 2);
    SPI1->CR1 |= (1U << 6); // SPE: SPI Enable
}

void SPI_Transmit(uint8_t data) {
    while (!(SPI1->SR & TXE)); // Wait for TX empty
    SPI1->DR = data;
    while (SPI1->SR & BUSY);   // Wait until not busy
}

uint8_t SPI_Receive(void) {
    while (!(SPI1->SR & TXE));
    SPI1->DR = 0xFF;           // Send dummy byte to trigger clock
    while (!(SPI1->SR & RXNE)); // Wait for RX buffer full
    return SPI1->DR;
}

int main() {
    char *name = "NITHISH";

    SPI1_Master_Init();
    lcd_init();

    // Example SPI Transaction
    GPIOA->ODR &= ~(1U << 4);  // CS Low
    SPI_Transmit(0x06);        // Example Command
    GPIOA->ODR |= (1U << 4);   // CS High

    delay(10);

    show_data(name);           // Display on LCD

    while (1);
}

/* ---------------- LCD FUNCTIONS ---------------- */

void lcd_init() {
    delay(20);
    send_cmd(0x38); // 8-bit mode
    send_cmd(0x0E); // Display ON, Cursor ON
    send_cmd(0x01); // Clear Display
    send_cmd(0x06); // Entry mode
}

void send_cmd(unsigned char cmd) {
    GPIOC->ODR = cmd;         // Data to Port C
    GPIOB->ODR &= ~RS;        // RS = 0
    GPIOB->ODR &= ~RW;        // RW = 0
    GPIOB->ODR |= EN;         // EN Pulse
    delay(2);
    GPIOB->ODR &= ~EN;
}

void send_data(unsigned char data) {
    GPIOC->ODR = data;        // Data to Port C
    GPIOB->ODR |= RS;         // RS = 1
    GPIOB->ODR &= ~RW;        // RW = 0
    GPIOB->ODR |= EN;         // EN Pulse
    delay(2);
    GPIOB->ODR &= ~EN;
}

void show_data(char *s) {
    while (*s) send_data(*s++);
}

void delay(int ms) {
    for (int i = 0; i < ms; i++)
        for (int j = 0; j < 4000; j++); // Approximation for F401
}
