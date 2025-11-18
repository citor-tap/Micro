#ifndef RNG_H
#define RNG_H

#include <stdint.h>
#include <stddef.h>

#define RNG_COUNT 200u

extern char     g_rng_digits[RNG_COUNT];  // Modo 1: 200 dígitos '0'..'9'
extern uint16_t g_rng_u16[RNG_COUNT];     // Modo 2: 200 enteros 0..999

void rng_seed(uint32_t seed);
void rng_mode1_generate(void);   // llena g_rng_digits
void rng_mode2_generate(void);   // llena g_rng_u16

#endif

