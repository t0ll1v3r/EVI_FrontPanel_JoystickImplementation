// keypad.h
#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

// matrix
#define KEYPAD_COLS		5
#define KEYPAD_ROWS		4

// key states
#define KEYPAD_PRESSED	1
#define KEYPAD_RELEASED	0


void keypad_init(void);		// initialize keypad GPIO (called once at startup)
void keypad_poll(void);		// scan keypad matrix

uint8_t kpd_getState(void);	// get current key press state (P or R)
uint8_t kpd_getCode(void);	// get HID code of last detected key

#endif