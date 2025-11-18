#include <stdint.h>
#include "met2.h"
#include "gen.h"
#include "led.h"
#include "usart.h"

extern void delay_ms(uint16_t t);  // definida en main.c

void metodo2(void)
{
    int i;
    const char *txt;

    // Inicio
    txt = "\r\nMet 2\r\n";
    while (*txt) {
        usart1_sendbyte((uint8_t)*txt);
        txt++;
    }

    led_off();  // arrancamos con el LED apagado

    // Enviar 200 números, en grupos de 5 por línea
    for (i = 0; i < CANTIDAD; i++) {

        usart1_sendbyte((uint8_t)buffer_m2[i]);

        // 5 números
        if ((i % 5) == 4) {
            usart1_sendbyte('\r');
            usart1_sendbyte('\n');

            led_toggle();
            delay_ms(60);  
        }
    }

    // Fin
    txt = "\r\n Fin 2\r\n";
    while (*txt) {
        usart1_sendbyte((uint8_t)*txt);
        txt++;
    }

    led_off(); 
}
