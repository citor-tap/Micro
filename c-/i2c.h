#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

/* --- API usada por tus drivers de OLED --- */
void     i2c2_init_100k(void);
int      i2c2_write_bytes(uint8_t addr7, const uint8_t *buf, uint16_t len);

/* --- API clásica (wrappers) --- */
void     init_i2c(void);
void     start_i2c(void);
void     stop_i2c(void);
void     address_i2c(uint8_t address7, uint8_t r_w); /* r_w: 0=write, 1=read */
void     data_i2c(uint8_t data);
void     write_i2c(uint8_t address7, uint8_t data[], uint64_t length);

#endif /* __I2C_H__ */
