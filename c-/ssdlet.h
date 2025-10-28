#ifndef __SSDLET_H__
#define __SSDLET_H__

#include <stdint.h>

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH    128u
#endif
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT    64u
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Dirección I2C 7-bit del SSD1306: REVISAR SI ES 0x3C u 0x3D */
void ssd_set_addr(uint8_t addr7);

/* Inicializa el panel (128x64), modo horizontal */
void ssd_init(void);

/* Borra por hardware todas las páginas/columnas (envía 0x00 a todo) */
void ssd_clear(void);

/* Fija cursor en coordenadas de bajo nivel:
 *  - columna y pagina, cada página = 8 px de alto la fuente que saltara de 2 en 2.
 */
void ssd_set_cursor(uint8_t x, uint8_t page);

void ssd_write_char_script(char c);

/* E'\n' para salto de línea
 */
void ssd_write_string_script(const char *s);

#ifdef __cplusplus
}
#endif
#endif

