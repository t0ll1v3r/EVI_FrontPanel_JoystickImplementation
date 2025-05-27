//#include <asf.h>
//#include <adc.h>
#include <avr/io.h>
#include <stdlib.h>

#include "joystick.h"

#define SLIDER_COUNT   24

static int8_t jstk_scan(uint16_t jstk_bits) 
{
    for (int8_t i = 0; i < SLIDER_COUNT; i++) {
        if ((jstk_bits & (1u << i)) == 0)
            return i;
    }
    return -1;
}


// vertical slider
static uint16_t jstk_readVertRaw(void)
{
    uint8_t jstk_c = PORTC.IN;
    uint8_t jstk_d = PORTD.IN;
    uint16_t jstk_w = ((uint16_t)jstk_d << 8) | jstk_c;
    jstk_w >>= 2;            // discard C0 & C1 (SDA/SCL)
    return jstk_w & 0x0FFF;  // keep only bits 0–11
}

int8_t jstk_readVertIndex(void) {
    int8_t idx = jstk_scan(jstk_readVertRaw());

    if (idx >= 0) {
        idx = 11 - idx;
    }

    return idx;
    // return jstk_scan(jstk_readVertRaw());
}

// horizontal slider
static uint16_t jstk_readHoriRaw(void) {
    uint8_t jstk_e = PORTE.IN;
    uint8_t jstk_b = PORTB.IN;
    uint16_t jstk_w = ((uint16_t)jstk_b << 8) | jstk_e;
    return jstk_w & 0x0FFF;  // B4–B7 land above bit11 and get masked off
}

int8_t jstk_readHoriIndex(void) {
    int8_t idx = jstk_scan(jstk_readHoriRaw());

    if (idx >= 0) {
        idx = 11 - idx;
    }
    
    return idx;
    // return jstk_scan(jstk_readHoriRaw());
}


uint8_t jstk_readMask(void) {
    int8_t vi = jstk_readVertIndex();
    int8_t hi = jstk_readHoriIndex();

    if (vi < 0 && hi < 0)
        return 0;

    uint8_t dV = (vi < 0) ? 0 : (vi > 5 ? vi - 5 : 5 - vi);
    uint8_t dH = (hi < 0) ? 0 : (hi > 5 ? hi - 5 : 5 - hi);

    int8_t jstk_use = (dV >= dH) ? vi : hi;

    return jstk_ledMask(jstk_use);
}

uint8_t jstk_ledMask(uint8_t idx) {
    if (idx < 0) {
        // no touch detected
        return 0;
    }

    // center dead‑zone
    if (idx == 5 || idx == 6) {
        return (1u<<3) | (1u<<4);  // LED4 (bit3) + LED5 (bit4)
    }

    // computes 'distance' from center [5,]
    uint8_t d = (idx < 5) ? (5 - idx) : (idx - 6);

    // controls how many LED's should activate
    uint8_t N = (d < 2 ? 2 : (d + 1));
    if (N > 4) {
        N = 4;
    }

    uint8_t jstk_mask = 0;
    if (idx < 5) {  // down/left LED joystick
        for (uint8_t i = 0; i < N; i++) {
            jstk_mask |= (1u << (3 - i));
        }
    } else {        // up/right LED joystick
        for (uint8_t i = 0; i < N; i++) {
            jstk_mask |= (1u << (4 + i));
        }
    }
    return jstk_mask;
}