#ifndef __EXT_IN_H__
#define __EXT_IN_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Flag levantado por la ISR de EXTI0 (PB0, flanco de subida) */
extern volatile uint8_t int0_flag;

/* Inicializa PB0 como entrada con pull-down, mapea EXTI0 y habilita la IRQ */
void init_ext_in(void);

#ifdef __cplusplus
}
#endif

#endif 


