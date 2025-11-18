#include "rng.h"

/* PRNG xorshift32 */
static uint32_t s_state = 0x12345678u;

static inline uint32_t xorshift32(void){
    uint32_t x = s_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s_state = x;
    return x;
}

char     g_rng_digits[RNG_COUNT];
uint16_t g_rng_u16[RNG_COUNT];

void rng_seed(uint32_t seed){
    s_state = (seed == 0u) ? 0x6C8E9CF5u : seed;
}

/* Modo 1: 200 dígitos '0'..'9' */
void rng_mode1_generate(void){
    for (size_t i = 0; i < RNG_COUNT; ++i){
        uint32_t r = xorshift32();
        uint32_t d = (uint32_t)(((uint64_t)r * 10ull) >> 32); // 0..9 sin sesgo
        g_rng_digits[i] = (char)('0' + d);
    }
}

/* Modo 2: 200 enteros 0..999 */
void rng_mode2_generate(void){
    for (size_t i = 0; i < RNG_COUNT; ++i){
        g_rng_u16[i] = (uint16_t)(xorshift32() % 1000u);
    }
}

