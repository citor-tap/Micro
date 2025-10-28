/************************/
#include "stm32f10x.h"
#include "gpio.h"

/* Macros de ayuda: nibbles de configuración CRL/CRH */
#define PIN_CFG(cr, shift, val)   do { (cr) &= ~(0xFU << (shift)); (cr) |= ((uint32_t)(val) << (shift)); } while (0)

/* Valores de nibble para F1:
 * - Salida 10MHz push-pull    => MODE=01, CNF=00 -> 0x1
 * - AF Open-Drain 50MHz       => MODE=11, CNF=11 -> 0xF
 * - Entrada con pull-up/down  => MODE=00, CNF=10 -> 0x8
 */

void init_ports(void)
{
    /* ---------- 1) Habilitar relojes ---------- */
    /* APB2: AFIO(0), IOPB(3), IOPC(4) */
    RCC->APB2ENR |= (1U << 0) | (1U << 3) | (1U << 4);
    /* APB1: I2C2(22) */
    RCC->APB1ENR |= (1U << 22);

    /* ---------- 2) I2C2 en PB10(SCL) y PB11(SDA) ----------
     * CRH nibbles:
     *  - PB10 -> bits [11:8]
     *  - PB11 -> bits [15:12]
     * AF Open-Drain 50 MHz => 0xF por pin
     */
    PIN_CFG(GPIOB->CRH, 8,  0xF);   /* PB10 SCL */
    PIN_CFG(GPIOB->CRH, 12, 0xF);   /* PB11 SDA */

    /* ---------- 3) LED en PC13 ----------
     * CRH nibbles:
     *  - PC13 -> bits [23:20]
     * Salida push-pull 10 MHz => 0x1
     */
    PIN_CFG(GPIOC->CRH, 20, 0x1);
    /* Muchas Blue Pill son activas en bajo: LED OFF = 1 */
    GPIOC->BSRR = (1U << 13);

    /* ---------- 4) Botón en PB0 con pull-up ----------
     * CRL nibbles:
     *  - PB0  -> bits [3:0]
     * Entrada con pull-up/down => 0x8 y luego ODR=1 para pull-up
     */
    PIN_CFG(GPIOB->CRL, 0, 0x8);
    GPIOB->ODR |= (1U << 0);

    /* (Opcional) Si quisieras mapear EXTI0 a PB0 aquí, sería vía AFIO->EXTICR[0].
       Lo dejo fuera para no mezclar GPIO con EXTI. */
}

