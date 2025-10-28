#include "stm32f10x.h"
#include "leds.h"

void leds_init(void)
{
    /*  PC13  salida en gpio_init() */
    GPIOC->BSRR = (1U << 13); // LED apagado
}

void leds_on(void)
{
    /* Enciende LED (activo en bajo) */
    GPIOC->BRR = (1U << 13);
}

void leds_off(void)
{
    /* Apaga LED (alto lógico) */
    GPIOC->BSRR = (1U << 13);
}

void leds_toggle(void)
{
    /* Cambia el estado del LED */
    GPIOC->ODR ^= (1U << 13);
}
