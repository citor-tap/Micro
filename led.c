#include <stm32f10x.h>
#include "led.h"

// LED 

void led_init(void)
{
    // Habilitar reloj
    RCC->APB2ENR |= (1 << 4);

    // PC13 salida, 10 MHz
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |=  (0x1 << 20);

    led_off();
}

void led_on(void)
{
    // reset bit 13 -> PC13 = 0 -> LED ON
    GPIOC->BSRR = (1 << 29);
}

void led_off(void)
{
    // set bit 13 -> PC13 = 1 -> LED OFF
    GPIOC->BSRR = (1 << 13);
}

void led_toggle(void)
{
    if (GPIOC->ODR & (1 << 13)) {
        led_on();
    } else {
        led_off();
    }
}
