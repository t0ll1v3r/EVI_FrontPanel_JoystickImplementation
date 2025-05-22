#include <asf.h>
#include <avr/io.h>
#include <conf_usb.h>
#include <util/delay.h>
#include <udi_hid_kbd.h>
#include "avr_compiler.h"

#include <string.h>
#include <stdbool.h>

#include "keypad.h"
#include "led.h"

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
Display Button	--> Column 2, Row 0	(HID_KEYPAD_5)
No Button		--> Column 2, Row 1	(0)
No Button		--> Column 2, Row 2	(0)
No Button		--> Column 2, Row 3	(0)
No Button		--> Column 3, Row 0	(0)
No Button		--> Column 3, Row 1	(0)
F1 Button		--> Column 3, Row 2	(HID_KEYPAD_1)
F3 Button		--> Column 3, Row 3	(HID_KEYPAD_2)
No Button		--> Column 4, Row 0	(0)
No Button		--> Column 4, Row 1	(0)
F2 Button		--> Column 4, Row 2	(HID_KEYPAD_3)
F4 Button		--> Column 4, Row 3	(HID_KEYPAD_4)
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

// scans the matrix, update lastState and lastCode
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


static kpd_stateT prevState;
static bool exitTestMode = false;

void keypadReport_init(void) {
	prevState = KEYPAD_RELEASED;
	exitTestMode = false;
}

void keypadReport_task(void)
{
	uint8_t curr = kpd_getState();
	uint8_t code = kpd_getCode();
	
	bool testMode = ((PORTB.IN & PIN4_bm) == 0);
	if (testMode) {
		if (curr == KEYPAD_PRESSED && prevState == KEYPAD_RELEASED)
		{
			uint8_t mask = 0;
			switch (code) 
			{
				case HID_KEYPAD_1:	mask = LED1_PIN;	break;	// F1
				case HID_KEYPAD_2:	mask = LED2_PIN;	break;	// F2
				case HID_KEYPAD_3:	mask = LED3_PIN;	break;	// F3
				case HID_KEYPAD_4:	mask = LED4_PIN;	break;	// F4
			
				case HID_KEYPAD_5:	mask = LED5_PIN;	break;	// Display
				case HID_KEYPAD_6:	mask = LED6_PIN;	break;	// Cancel
				case HID_KEYPAD_7:	mask = LED7_PIN;	break;	// Enter
				case HID_KEYPAD_8:	mask = LED8_PIN;	break;	// Clear
				case HID_KEYPAD_9:	mask = LED1_PIN;	break;	// Null
				default:			mask = 0;			break;
			}
			if (mask) led_toggle(mask);
			exitTestMode = true;
		}
		else
		{
			if (exitTestMode) {
				led_allOff();
				exitTestMode = false;
			}
			if (curr == KEYPAD_PRESSED && prevState == KEYPAD_RELEASED) {
				udi_hid_kbd_down(code);
			} else if (curr == KEYPAD_RELEASED && prevState == KEYPAD_PRESSED) {
				udi_hid_kbd_up(code);
			}
		}
	}
	prevState = curr;
}


// BRANCH: USB_ATTEMPT3