#ifndef JOYSTICK_H
#define JOYSTICK_H

// #include <asf.h>  // brings in device headers for XMEGA
// #include <avr/io.h>
#include <stdint.h>


// function prototypes
// void jstk_read(uint8_t *xPercent, uint8_t *yPercent);
int8_t jstk_readVertIndex(void);
int8_t jstk_readHoriIndex(void);
uint8_t jstk_readMask(void);

uint8_t jstk_ledMask(uint8_t percent);

#endif // JOYSTICK_H