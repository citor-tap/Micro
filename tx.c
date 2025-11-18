#include <stm32f10x.h>
#include "tx.h"
#include "rng.h"

/* Declaramos estas funciones del main como externas */
extern void usart1_sendbyte(unsigned char c);
extern void delay_ms(uint16_t t);

/* ===== LED PC13 (Blue Pill) =====
 * LED activo en bajo:
 *  - 0 en PC13  -> LED ON
 *  - 1 en PC13  -> LED OFF
 */
static void led_init(void){
    RCC->APB2ENR |= (1u<<4);         // GPIOC clock
    GPIOC->CRH &= ~(0xFu << 20);     // limpia config de PC13
    GPIOC->CRH |=  (0x1u << 20);     // salida push-pull, 10 MHz
    // Apaga LED inicialmente (bit=1)
    GPIOC->BSRR = (1u << 13);
}

static inline void led_on(void){
    GPIOC->BSRR = (1u << 29);        // reset bit 13 -> 0 -> LED ON
}

static inline void led_off(void){
    GPIOC->BSRR = (1u << 13);        // set bit 13 -> 1 -> LED OFF
}

static inline void led_toggle(void){
    if (GPIOC->ODR & (1u<<13)) led_on();
    else                       led_off();
}

/* ========= MODO 1 =========
 * - Genera 200 dígitos '0'..'9'
 * - Enciende LED fijo
 * - Envía: "Inicio modo 1", los 200 chars, "Fin"
 */
void modo1(void){
    led_init();
    led_on();               // LED fijo encendido

    rng_mode1_generate();   // llena g_rng_digits[200]

    // "Inicio modo 1\r\n"
    const char *ini = "Inicio modo 1\r\n";
    for (const char *p = ini; *p; ++p)
        usart1_sendbyte((unsigned char)*p);

    // Enviar los 200 dígitos, estilo tu main (byte a byte)
    for (int i = 0; i < RNG_COUNT; ++i)
        usart1_sendbyte((unsigned char)g_rng_digits[i]);

    // "\r\nFin\r\n"
    const char *fin = "\r\nFin\r\n";
    for (const char *p = fin; *p; ++p)
        usart1_sendbyte((unsigned char)*p);

    // LED queda encendido al terminar modo 1
}

/* ========= MODO 2 =========
 * - Genera 200 enteros 0..999
 * - LED parpadeando mientras se envían
 * - Envía: "Inicio modo 2", los 200 números (uno por línea), "Fin"
 */
void modo2(void){
    led_init();
    led_off();              // inicia apagado

    rng_mode2_generate();   // llena g_rng_u16[200]

    const char *ini = "Inicio modo 2\r\n";
    for (const char *p = ini; *p; ++p)
        usart1_sendbyte((unsigned char)*p);

    // Para cada número:
    for (int i = 0; i < RNG_COUNT; ++i){
        uint16_t v = g_rng_u16[i];

        // convertir v a ASCII decimal
        char buf[8];
        int p = 0;

        if (v == 0){
            buf[p++] = '0';
        } else {
            char tmp[5];
            int t = 0;
            while (v && t < 5){
                tmp[t++] = (char)('0' + (v % 10u));
                v /= 10u;
            }
            while (t--)
                buf[p++] = tmp[t];
        }
        buf[p++] = '\r';
        buf[p++] = '\n';

        // enviar caracteres
        for (int k = 0; k < p; ++k)
            usart1_sendbyte((unsigned char)buf[k]);

        // parpadeo del LED cada número
        led_toggle();
        delay_ms(30);   // ajusta para parpadeo más lento/rápido
    }

    const char *fin = "Fin\r\n";
    for (const char *p = fin; *p; ++p)
        usart1_sendbyte((unsigned char)*p);

    // Deja LED encendido al final
    led_on();
}


