/*| Funci�n 										                                    || Fuente 											                         |
| ----------------------------------------------------------------- || ----------------------------------------------------- |
| `SystemInit()`                                                    || CMSIS / ST Standard Peripheral Template               |
| `__NOP()`                                                         || ARM CMSIS Macro                                       |
| `gpio_init()` / `init_ports()`                                    || Ejemplos SPL + RM0008 cap. 9                          |
| `leds_init()`, `leds_toggle()`                                    || C�digo propio (basado en GPIO)                        |
| `i2c2_init_100k()`, `i2c2_write_bytes()`                          || RM0008 cap. 27 (I�C)                                  |
| `init_ext_in()`, `EXTI0_IRQHandler()`                             || RM0008 cap. 10 (EXTI) + CMSIS                         |
| `ssd_detect_addr()`, `ssd_init()`, `ssd_clear()`, `ssd_write_*()` || Datasheet SSD1306 (Solomon Systech)                   |
| `fonts.c`, `fontscript_find()`                                    || C�digo propio (basado en idea de Adafruit GFX / u8g2) |
| `oled_show_boot()`                                                || Propio (funci�n de aplicaci�n)                        |
| `for(;;)` loop con `leds_toggle()` y `ssd_write_string_script()`  || Propio (aplicaci�n principal)                         |
*/

#include "stm32f10x.h"
#include <stdint.h>
#include "gpio.h"
#include "leds.h"
#include "i2c.h"
#include "ext_in.h"
#include "ssdlet.h"

// ====== Texto que quieres mostrar ======
#define TEXT_LINE1   "CITLALLI YAEL"
#define TEXT_LINE2   "ACTIVIDAD 4"

// ====== Deteccion de direccion SSD1306 ======
static uint8_t ssd_detect_addr(void)
{
    uint8_t pkt[2] = {0x00, 0xAE}; // (CTRL_CMD, DISPLAY OFF)
    int n;

    ssd_set_addr(0x3C);
    n = i2c2_write_bytes(0x3C, pkt, 2);
    if (n == 2) return 0x3C;

    ssd_set_addr(0x3D);
    n = i2c2_write_bytes(0x3D, pkt, 2);
    if (n == 2) return 0x3D;

    return 0x3C; 
}

static void oled_show_boot(void)
{
    ssd_clear();
    ssd_set_cursor(0, 0);  // p�gina 0
    ssd_write_string_script(TEXT_LINE1);

    ssd_set_cursor(0, 2);  // +16 px (dos p�ginas)
    ssd_write_string_script(TEXT_LINE2);
}

int main(void)
{
    SystemInit();

    gpio_init();        // o init_ports(); (alias en gpio.h)
    leds_init();
    i2c2_init_100k();
    init_ext_in();      // <-- ahora s� declarado y definido

    uint8_t addr = ssd_detect_addr();
    ssd_set_addr(addr);
    ssd_init();
    oled_show_boot();

    for (;;)
    {
        /* PB0 flanco de subida -> EXTI0_IRQHandler() pone int0_flag=1 */
        if (int0_flag)
        {
            int0_flag = 0;

            leds_toggle();
            ssd_set_cursor(0, 2);
            ssd_write_string_script("BTN!");
        }

        __NOP(); // loop sin bloqueos
    }
}
