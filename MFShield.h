

// See README.md for detailed description


#ifndef MFSHIELD_H
#define MFSHIELD_H

#include <Arduino.h>

/* SHIELD HARDWARE PIN DEFINITIONS */

#define MFS_PIN_DISP_CK 7
#define MFS_PIN_DISP_CS 4
#define MFS_PIN_DISP_DO 8
#define MFS_PIN_TRIMMER A0
#define MFS_PIN_BUZZER 3

/*	---------------------------------
	SYSTEM CONSTANTS
	(do not touch unless necessary) */

#define MFS_DISP_SCAN_INTERVAL_MS 1
/*	Adjust a scan rate for the display in msec per digit.
	Example: if the value is set to 1 ms, the output refresh rate will be:
		1ms * 4 digits = 4 ms or 250 Hz
	Higher interval means a slightly less load, but cause more flickering.
	Reccommended value: 1..5 ms  */

#define MFS_BUTTONS_POLLING_INTERVAL_MS 20
/*	Polling interval for buttons
	Higher values give a better debounce but also increase the response time.
	Recommended value: 20..100 ms  */

/* CLASS DEFINITION */

class MFShield
{
	public:
		MFShield ();
		
		// Assign a button press event callback 
		// NOTE: the function must have an argument (8-bit variable) to receive the number of button is being pressed
		void onKeyPress (void (*func)(uint8_t key));	

		uint16_t trimmer();				// read onboard potentiometer value
		uint16_t getTrimmerValue ();	// idem

		void setLed (uint8_t led, boolean state);
		boolean getLed (uint8_t led);

		void beep (uint16_t ms = 100);	// emit a sound for a specific duration

		void display (int16_t val, boolean leading_zero = false); 	// show the number on display
		//void display (float val, boolean leading_zero = false);

		void showDisplay (boolean visible = true);		// turns on and off the numeric display

		/* [!] Don't forget to insert this function in the main loop: */
		void loop ();	

	private:

		void (*onKeyPress_func)(uint8_t);

		uint8_t key_mask_pre;

		uint32_t t_disp;
		uint32_t t_buttons;
		uint32_t t_beep;

		uint8_t raw_segment [4];
		uint8_t current_segment;
		boolean disp_enable;
		boolean disp_leading_zero;

		static const uint8_t LED_PIN [];
		static const uint8_t KEY_PIN [];
		static const uint8_t SEG_MAP [];
		static const uint8_t SEG_SEL [];
};


#endif