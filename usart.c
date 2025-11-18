#include <stm32f10x.h>
#include "usart.h"

void usart1_init(void)
{
    // Activar reloj de USART1, GPIOA y AFIO
    RCC->APB2ENR |= (1 << 14) | (1 << 2) | (1 << 0);

    // PA10 = RX1 entrada con pull-up
    GPIOA->CRH &= ~((1 << 9) | (1 << 8)); // modo entrada
    GPIOA->CRH |=  (1 << 11);             // CNF10 = 10 -> input pull up/down
    GPIOA->ODR  |=  (1 << 10);            // pull-up

    // PA9 = TX1 salida AF push-pull 50 MHz
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);            // 1011b: AF push-pull, 50MHz

    // Habilitar USART1, TX y RX
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);

    // *** Asumimos 72 MHz (SystemInit típico) ***
    // Baud rate = 9600 bps ? BRR = 7500
    USART1->BRR = 7500;
}

void usart1_sendbyte(uint8_t c)
{
    USART1->DR = c;
    while ((USART1->SR & (1 << 6)) == 0); // esperar TC
    USART1->SR &= ~(1 << 6);              // limpiar TC
}
