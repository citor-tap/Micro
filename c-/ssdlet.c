#include <stdint.h>
#include "i2c.h"
#include "ssdlet.h"
#include "fonts.h"

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 128
#endif
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif


#ifdef USE_SH1106
#  define COL_OFFSET 2
#else
#  define COL_OFFSET 0
#endif

#define CTRL_CMD   0x00u
#define CTRL_DATA  0x40u

static uint8_t g_addr7  = 0x3C;
static uint8_t g_cur_x  = 0;
static uint8_t g_cur_pg = 0;

/* ============================================================
 * I2C helpers */
static inline void ssd__send_cmd(uint8_t c)
{
    uint8_t pkt[2] = { CTRL_CMD, c };
    (void)i2c2_write_bytes(g_addr7, pkt, 2);
}
static inline void ssd__send_data(uint8_t d)
{
    uint8_t pkt[2] = { CTRL_DATA, d };
    (void)i2c2_write_bytes(g_addr7, pkt, 2);
}
static inline void ssd__set_x_page(uint8_t x, uint8_t page)
{
    uint8_t xx = (uint8_t)(x + COL_OFFSET);
    ssd__send_cmd((uint8_t)(0xB0 | (page & 0x07)));
    ssd__send_cmd((uint8_t)(0x00 | (xx & 0x0F)));
    ssd__send_cmd((uint8_t)(0x10 | (xx >> 4)));
}
static inline void ssd__newline_16(void)
{
    g_cur_x = 0;
    g_cur_pg = (uint8_t)((g_cur_pg + 2) & 7u);
    ssd__set_x_page(g_cur_x, g_cur_pg);
}

/* ================================== */
static inline void ssd__draw_col16(uint16_t col16)
{
    uint8_t low8  = (uint8_t)(col16 & 0xFFu);
    uint8_t high8 = (uint8_t)((col16 >> 8) & 0xFFu);

    ssd__set_x_page(g_cur_x, g_cur_pg);
    ssd__send_data(low8);
    ssd__set_x_page(g_cur_x, (uint8_t)(g_cur_pg + 1u));
    ssd__send_data(high8);

    g_cur_x++;
    if (g_cur_x >= SSD1306_WIDTH) ssd__newline_16();
}

/* ============================================== */
void ssd_set_addr(uint8_t addr7) { g_addr7 = (addr7 & 0x7Fu); }

void ssd_init(void)
{
    ssd__send_cmd(0xAE);
    ssd__send_cmd(0x20); ssd__send_cmd(0x00); /* modo horizontal */
    ssd__send_cmd(0xA1); ssd__send_cmd(0xC8); /* remaps normales */
    ssd__send_cmd(0xB0);
    ssd__send_cmd(0x00);
    ssd__send_cmd(0x10);
    ssd__send_cmd(0x40);
    ssd__send_cmd(0x81); ssd__send_cmd(0x7F);
    ssd__send_cmd(0xA6);
    ssd__send_cmd(0xA8); ssd__send_cmd(0x3F);
    ssd__send_cmd(0xA4);
    ssd__send_cmd(0xD3); ssd__send_cmd(0x00);
    ssd__send_cmd(0xD5); ssd__send_cmd(0x80);
    ssd__send_cmd(0xD9); ssd__send_cmd(0xF1);
    ssd__send_cmd(0xDA); ssd__send_cmd(0x12);
    ssd__send_cmd(0xDB); ssd__send_cmd(0x40);
    ssd__send_cmd(0x8D); ssd__send_cmd(0x14);
    ssd__send_cmd(0xAF);
    ssd__send_cmd(0x21); ssd__send_cmd(0); ssd__send_cmd(127);
    ssd__send_cmd(0x22); ssd__send_cmd(0); ssd__send_cmd(7);
    ssd_clear();
    ssd_set_cursor(0,0);
}

void ssd_clear(void)
{
    for (uint8_t p=0; p<8u; ++p) {
        ssd__set_x_page(0,p);
        for (uint8_t x=0; x<SSD1306_WIDTH; ++x) ssd__send_data(0x00);
    }
    g_cur_x=0; g_cur_pg=0;
    ssd__set_x_page(0,0);
}

void ssd_set_cursor(uint8_t x, uint8_t page)
{
    if (x>=SSD1306_WIDTH) x=0;
    if (page>7u) page=0;
    if (page&1u) page--;
    g_cur_x=x; g_cur_pg=page;
    ssd__set_x_page(g_cur_x,g_cur_pg);
}

/* ============================================================
 * Render de caracteres 
	================================== */
void ssd_write_char_script(char c)
{
    const FontScriptGlyph* g = fontscript_find(c);
    if (!g) return;

    for (uint8_t i=0; i<g->width; ++i){
        uint16_t v16 = g->cols[i];
        ssd__draw_col16(v16);
    }

    /* columna separadora */
    ssd__set_x_page(g_cur_x, g_cur_pg); ssd__send_data(0x00);
    ssd__set_x_page(g_cur_x, (uint8_t)(g_cur_pg+1u)); ssd__send_data(0x00);
    g_cur_x++;
    if (g_cur_x>=SSD1306_WIDTH) ssd__newline_16();
}

void ssd_write_string_script(const char *s)
{
    while (s && *s) {
        char c = *s++;
        if (c == '\n'){ ssd__newline_16(); continue; }
        ssd_write_char_script(c);
    }
}
