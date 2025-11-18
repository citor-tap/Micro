#include <stdint.h>
#include "met.h"
#include "gen.h"
#include "usart.h"
#include "led.h"

extern void delay_ms(uint16_t t);  // definida en main.c

void metodo1(void)
{
    int i;
    const char *txt;

    led_on();   // LED fijo encendido durante todo el modo 1

    // Mensaje de inicio
    txt = "\r\nMet 1\r\n";
    while (*txt) {
        usart1_sendbyte((uint8_t)*txt);
        txt++;
    }

    // Enviar los 200 números desde buffer_m1 (en SRAM), sin espacios
    for (i = 0; i < CANTIDAD; i++) {
        usart1_sendbyte((uint8_t)buffer_m1[i]);
    }

    // Mensaje de fin
    txt = "\r\nFin 1\r\n";
    while (*txt) {
        usart1_sendbyte((uint8_t)*txt);
        txt++;
    }

    // El LED queda encendido al terminar
}

