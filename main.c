#include <stm32f10x.h>
#include <stdint.h>

#include "usart.h"
#include "gen.h"
#include "met.h"
#include "met2.h"
#include "led.h"

void delay_ms(uint16_t t);

int main(void)
{
    led_init();      
    usart1_init();   // configurar USART1 

    // Generar num guarda en SRAM
    generar_m1();    // buffer_m1[200]
    generar_m2();    // buffer_m2[200]

    // Llama modo 1
    metodo1();

    // Intermedio
    delay_ms(1500);

    // Llama modo 2
    metodo2();

    // Bucle infinito
    while (1) {
        
    }
}

// Retardo 
void delay_ms(uint16_t t)
{
    volatile unsigned long i;
    while (t--) {
        for (i = 0; i < 6000; i++) {
            // vacío
        }
    }
}

