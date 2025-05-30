#include <asf.h>
#include "conf_usb.h"
#include "avr_compiler.h"

#include "keypad.h"
#include "led.h"
#include "io.h"


static volatile bool main_b_kbd_enable = false;


int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init();			// initialize sleep manager
	sysclk_init();				// initialize clock
	
	// written by Uniwest
	io_init();
	keypad_init();				// initializes keypad functionality
	// led_init();					// initializes LED functionality

	udc_start();				// start USB stack to authorize VBus monitoring

	while (true)
	{
		keypad_poll();
		keypad_report();
	}
}

void main_suspend_action(void) { }
void main_resume_action(void) { }

void main_sof_action(void) {
	if (!main_b_kbd_enable)
		return;
	BD76319_ui_process(udd_get_frame_number());
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