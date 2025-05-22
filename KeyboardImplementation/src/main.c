/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <string.h>

#include "conf_usb.h"
#include "avr_compiler.h"
#include <util/delay.h>

//#include "udi_hid_kbd.h"
#include <udi_hid_kbd.h>

//#include "init_io.h"
#include "keypad.h"
#include "led.h"

typedef struct {
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
	} kybd_report_t;


static volatile bool main_b_kbd_enable = false;


int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init();			// initialize sleep manager
	sysclk_init();				// initialize clock
	
	// written by Uniwest
	keypad_init();				// initializes keypad functionality
	led_init();					// initializes LED functionality

	udc_start();				// start USB stack to authorize VBus monitoring

	// testing mode
	PORTB.DIRCLR = PIN4_bm;
	PORTB.PIN4CTRL = PORT_OPC_PULLUP_gc;
	static volatile uint8_t testMode;
	
	// normal mode
	kybd_report_t report;
	memset(&report, 0, sizeof(report));
	
	uint8_t prev = KEYPAD_RELEASED;
	while (true)
	{
		keypad_poll();
		uint8_t curr = kpd_getState();
		uint8_t code = kpd_getCode();
		
		testMode = PORTB.IN;
		uint8_t exitTestMode = 0;
		
		if (curr == KEYPAD_PRESSED && prev == KEYPAD_RELEASED)
		{
			
			if ((testMode & 0x010) == 0) {
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
			
				if (mask) {	// upon button press, the assigned LED will blink on
					led_toggle(mask);
				}
				
				exitTestMode = 1;				
			}
		}
		
		if (((testMode & 0x010) != 0) && (exitTestMode = 1)) {
			led_allOff();
			exitTestMode = 0;
		}
		
		prev = curr;
		_delay_ms(1);
	}
}

void main_suspend_action(void) { }
	
void main_resume_action(void) { }

void main_sof_action(void) {
	if (!main_b_kbd_enable) {
		return;
	}
}

void main_remotewakeup_enable(void) { }

void main_remotewakeup_disable(void) { }

bool main_kbd_enable(void) {
	main_b_kbd_enable = true;
	return true;
}

void main_kbd_disable(void) {
	main_b_kbd_enable = false;
}


// TESTING GIT BRANCH