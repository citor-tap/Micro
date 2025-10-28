#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Inicializa:
 * - Clocks: GPIOB, GPIOC, AFIO, I2C2
 * - PB10/PB11: I2C2 (AF Open-Drain 50 MHz)
 * - PC13: salida push-pull 10 MHz (LED OFF por defecto)
 * - PB0: entrada con pull-up (botón)
 */
void init_ports(void);

/* Alias por compatibilidad con otros main.c */
static inline void gpio_init(void) { init_ports(); }

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */
