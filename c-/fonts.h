#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

/* M�ximo ancho  que usa un glifo en fuente */
#define FONT_SCRIPT_MAX_COLS  11   
/* Cada columna es un uint16 */
typedef struct {
    char     ch;                               /* Car�cter ASCII representado */
    uint8_t  width;                            /* Columnas v�lidas en cols[]   */
    uint16_t cols[FONT_SCRIPT_MAX_COLS];       /* Columnas      */
} FontScriptGlyph;

#ifdef __cplusplus
extern "C" {
#endif
    /* *********** */
    const FontScriptGlyph* fontscript_find(char c);
#ifdef __cplusplus
}
#endif

#endif 