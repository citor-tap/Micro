#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
void leds_init(void);
void leds_on(void);
void leds_off(void);
void leds_toggle(void);

#ifdef __cplusplus
}
#endif

#endif 
