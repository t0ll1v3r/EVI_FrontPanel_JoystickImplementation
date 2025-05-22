#include <asf.h>
#include <string.h>

#include "conf_usb.h"
#include <util/delay.h>
#include <udi_hid_kbd.h>
#include "avr_compiler.h"

#include "keypad.h"
#include "led.h"


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
	keypadReport_init();		// prepares HID report state


	uint8_t prev = KEYPAD_RELEASED;
	while (true)
	{
		keypad_poll();
		keypadReport_task();
		_delay_ms(1);
	}
}

void main_suspend_action(void) { }
void main_resume_action(void) { }

void main_sof_action(void) {
	if (!main_b_kbd_enable) return;
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


// BRANCH: USB_ATTEMPT3