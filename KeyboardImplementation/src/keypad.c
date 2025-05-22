#include "avr_compiler.h"
#include <avr/io.h>
#include <util/delay.h>
#include <asf.h>

#include <stdio.h>

#include <keypad.h>
#include <conf_usb.h>

static uint8_t ui_KeyStatusToReport;

// Keypad
/*
NULL Button		--> Column 0, Row 0	(HID_KEYPAD_9)
CLEAR Button	--> Column 0, Row 1	(HID_KEYPAD_8)
No Button		--> Column 0, Row 2	(0)
No Button		--> Column 0, Row 3	(0)
ENTER Button	--> Column 1, Row 0	(HID_KEYPAD_7)
CANCEL Button	--> Column 1, Row 1	(HID_KEYPAD_6)
No Button		--> Column 1, Row 2	(0)
No Button		--> Column 1, Row 3	(0)
Display Button	--> Column 2, Row 0	(HID_KEYPAD_5) (HID_F5)
No Button		--> Column 2, Row 1	(0)
No Button		--> Column 2, Row 2	(0)
No Button		--> Column 2, Row 3	(0)
No Button		--> Column 3, Row 0	(0)
No Button		--> Column 3, Row 1	(0)
F1 Button		--> Column 3, Row 2	(HID_KEYPAD_1) (HID_F1)
F3 Button		--> Column 3, Row 3	(HID_KEYPAD_2) (HID_F3)
No Button		--> Column 4, Row 0	(0)
No Button		--> Column 4, Row 1	(0)
F2 Button		--> Column 4, Row 2	(HID_KEYPAD_3) (HID_F2)
F4 Button		--> Column 4, Row 3	(HID_KEYPAD_4) (HID_F4)
*/

static const uint8_t kpd_KeyMap[KEYPAD_COLS][KEYPAD_ROWS] = {
	{ HID_KEYPAD_9,	HID_KEYPAD_8,	0,				0				},
	{ HID_KEYPAD_7,	HID_KEYPAD_6,	0,				0				},
	{ HID_KEYPAD_5,	0,				0,				0				},
	{ 0,			0,				HID_KEYPAD_1,	HID_KEYPAD_3	},
	{ 0,			0,				HID_KEYPAD_2,	HID_KEYPAD_4	}
};
	
static const uint8_t kpd_ColAddr[KEYPAD_COLS] = {
	0x0E, 0x0D, 0x0B, 0x07, 0xFF
};

static volatile uint8_t lastState = KEYPAD_RELEASED;
static volatile uint8_t lastCode = 0;


void keypad_init(void) {
	// PF0-PF3 as outputs
	PORTF.DIRSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm);
	PORTF.OUTSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm);
	
	// PF4-PF7 as inputs
	PORTF.DIRCLR = (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);
	PORTF.PIN4CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN5CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN7CTRL = PORT_OPC_PULLUP_gc;
	
	// PB7 as output
	PORTB.DIRSET = PIN7_bm;
	PORTB.OUTSET = PIN7_bm;
}


void keypad_poll(void) {
	uint8_t col, row_r;
	int8_t detectedCol = -1, detectedRow = -1;
	
	for (col = 0; col < KEYPAD_COLS; col++) {
		PORTF.OUT = kpd_ColAddr[col];
		if (col == KEYPAD_COLS - 1) {
			PORTB.OUTCLR = PIN7_bm;
		} else {
			PORTB.OUTSET = PIN7_bm;
		}
		_delay_us(5);
		row_r = (PORTF.IN & 0XF0);
		
		switch (row_r) {
			case 0xE0: detectedRow = 0; detectedCol = col; break;
			case 0xD0: detectedRow = 1; detectedCol = col; break;
			case 0xB0: detectedRow = 2; detectedCol = col; break;
			case 0x70: detectedRow = 3; detectedCol = col; break;
			default: break;
		}
		if (detectedCol >= 0) {
			break;
		}
	}
	
	if (detectedCol >= 0) {
		_delay_us(100);
		lastCode = kpd_KeyMap[detectedCol][detectedRow];
		lastState = KEYPAD_PRESSED;
	} else {
		_delay_us(100);
		lastState = KEYPAD_RELEASED;
	}
}

uint8_t kpd_getState(void) {
	return lastState;
}

uint8_t kpd_getCode(void) {
	return lastCode;
}