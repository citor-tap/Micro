/******************************/
#include "stm32f10x.h"
#include "i2c.h"

/* Suponiendo PCLK1 = 36 MHz (Blue Pill a 72 MHz, APB1/2 por defecto) */
#ifndef I2C2_PCLK_MHZ
#define I2C2_PCLK_MHZ   36U
#endif

/* -------- Helpers internos (estilo compacto) -------- */
static inline void _i2c2_start(void){
    I2C2->CR1 |= I2C_CR1_START;
    while(!(I2C2->SR1 & I2C_SR1_SB)) { /* espera */ }
    (void)I2C2->SR1; /* clear SB */
}
static inline void _i2c2_addr(uint8_t addr7, uint8_t rw){
    I2C2->DR = (uint8_t)((addr7 << 1) | (rw & 1U));
    while(!(I2C2->SR1 & I2C_SR1_ADDR)) { /* espera */ }
    (void)I2C2->SR1; (void)I2C2->SR2; /* clear ADDR */
}
static inline void _i2c2_stop(void){
    I2C2->CR1 |= I2C_CR1_STOP;
}

/* =========================================================
 *                API “bonita” (drivers OLED)
 * ========================================================= */
void i2c2_init_100k(void)
{
    /* Reloj del periférico */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    /* Reset suave y limpio */
    I2C2->CR1 = I2C_CR1_SWRST;
    I2C2->CR1 = 0;

    /* FREQ en MHz (obligatorio antes de CCR/TRISE) */
    I2C2->CR2 = I2C2_PCLK_MHZ;   /* 36 MHz */

    /* Modo estándar 100 kHz: CCR = Fpclk1 / (2*Fscl) */
    uint16_t ccr = (uint16_t)((I2C2_PCLK_MHZ * 1000000U) / (2U * 100000U));
    if (ccr < 4U) ccr = 4U;      /* por seguridad */
    I2C2->CCR = ccr;

    /* TRISE = Fpclk1(MHz) + 1 para modo estándar */
    I2C2->TRISE = I2C2_PCLK_MHZ + 1U;

    /* Habilitar periférico */
    I2C2->CR1 |= I2C_CR1_PE;
}

int i2c2_write_bytes(uint8_t addr7, const uint8_t *buf, uint16_t len)
{
    if (!len) return 0;

    /* Esperar bus libre */
    while (I2C2->SR2 & I2C_SR2_BUSY) { /* espera */ }

    _i2c2_start();
    _i2c2_addr(addr7, 0); /* write */

    for (uint16_t i = 0; i < len; ++i) {
        while(!(I2C2->SR1 & I2C_SR1_TXE)) { /* espera */ }
        I2C2->DR = buf[i];
        if (i < (len - 1U)) {
            while(!(I2C2->SR1 & I2C_SR1_BTF)) { /* espera */ }
        }
    }
    while(!(I2C2->SR1 & I2C_SR1_BTF)) { /* asegura último byte */ }

    _i2c2_stop();
    return (int)len;
}

/* =========================================================
 *                API “clásica” (wrappers)
 * ========================================================= */
void init_i2c(void)
{
    /* Simplemente llama a la config de 100 kHz ya probada */
    i2c2_init_100k();
}

void start_i2c(void)
{
    _i2c2_start();
}

void stop_i2c(void)
{
    _i2c2_stop();
}

void address_i2c(uint8_t address7, uint8_t r_w)
{
    _i2c2_addr(address7, r_w);
}

void data_i2c(uint8_t data)
{
    while(!(I2C2->SR1 & I2C_SR1_TXE)) { /* espera */ }
    I2C2->DR = data;
}

void write_i2c(uint8_t address7, uint8_t data[], uint64_t length)
{
    if (!length) return;

    while (I2C2->SR2 & I2C_SR2_BUSY) { /* espera bus libre */ }

    _i2c2_start();
    _i2c2_addr(address7, 0); /* write */

    for (uint64_t i = 0; i < length; ++i) {
        data_i2c(data[i]);
        if (i + 1U < length) {
            while(!(I2C2->SR1 & I2C_SR1_BTF)) { /* espera */ }
        }
    }
    while(!(I2C2->SR1 & I2C_SR1_BTF)) { /* asegura último byte */ }
    _i2c2_stop();
}
