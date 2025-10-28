#include "stm32f10x.h"
#include "ext_in.h"

volatile uint8_t int0_flag = 0;

/* Helper configur nibbles*/
#define PINCFG(reg, shift, val)  do { (reg) &= ~(0xFU << (shift)); (reg) |= ((uint32_t)(val) << (shift)); } while (0)
/* Entrada pull-up/down en F1: MODE=00, CNF=10 => 0x8 */

void init_ext_in(void)
{
    /* 1) Clocks: AFIO y GPIOB */
    RCC->APB2ENR |= (1U << 0) /* AFIO */ | (1U << 3) /* IOPB */;

    /* 2) PB0 como entrada con pull-**down** */
    PINCFG(GPIOB->CRL, 0, 0x8);
    GPIOB->ODR &= ~(1U << 0);   /* ODR=0 -> pull-down */

    /* 3) Mapear EXTI0 a PB0 */
    AFIO->EXTICR[0] &= ~(0xFU << 0);
    AFIO->EXTICR[0] |=  (0x1U << 0);  /* 0001 = Port B */

    /* 4) EXTI0: unmask + rising edge, sin falling */
    EXTI->IMR  |=  (1U << 0);
    EXTI->RTSR |=  (1U << 0);
    EXTI->FTSR &= ~(1U << 0);

    /* 5) NVIC: habilitar IRQ */
    NVIC_EnableIRQ(EXTI0_IRQn);

    /* Limpiar pendientes previas */
    EXTI->PR = (1U << 0);
}

/* ISR: limpia pendiente y levanta bandera */
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0)) {
        EXTI->PR = (1U << 0);   /* acknowledge */
        int0_flag = 1U;
    }
}
