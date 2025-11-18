#include "gen.h"

char buffer_m1[CANTIDAD];   // 
char buffer_m2[CANTIDAD];   //

// PRNG muy sencillo tipo LCG, estilo estudiante
static unsigned int semilla1 = 12345;
static unsigned int semilla2 = 54321;

void generar_m1(void)
{
    unsigned int x = semilla1;
    int i;

    for (i = 0; i < CANTIDAD; i++) {
        x = x * 3 + 7;              // Formula 1
        buffer_m1[i] = '0' + (x % 10); 
    }

    semilla1 = x; 
}

void generar_m2(void)
{
    unsigned int x = semilla2;
    int i;

    for (i = 0; i < CANTIDAD; i++) {
        x = x * 5 + 11;             // Formula 2
        buffer_m2[i] = '0' + (x % 10);
    }

    semilla2 = x;
}
