
#ifndef MFSHIELD_H
#define MFSHIELD_H

#include <Arduino.h>
/*
	See examples for detailed description
	TODO:
		- Display negative and floating point numbers
		- Binary level optimisation (use port registers instead of digitalWrite())
*/


/* SHIELD HARDWARE PIN DEFINITIONS */

#define MFS_PIN_DISP_CK 7
#define MFS_PIN_DISP_CS 4
#define MFS_PIN_DISP_DO 8
#define MFS_PIN_TRIMMER A0
#define MFS_PIN_BUZZER 3

/*	SYSTEM CONSTANTS
	(do not touch unless necessary) */

/*	Scan rate of the onboard 4-digit display
	higher interval means slightly less system load,
	but causes more flickering.
	This interval means milliseconds to update ONE
	segment of display.
	(i.e. 1ms * 4 digits = 4 ms or 250 Hz)
	Reccommended value: 1..5 ms */
#define MFS_DISP_SCAN_INTERVAL_MS 1

/*	Polling interval for buttons
	Recommended value: 20-100 ms */
#define MFS_BUTTONS_POLLING_INTERVAL_MS 20


/* CLASS DEFINITION */

class MFShield
{
	public:
		MFShield ();
		
		void onKeyPress (void (*func)(uint8_t key));
		void onKeyRelease (void (*func)());
		uint16_t getTrimmerValue ();

		void setLed (uint8_t led, boolean state);
		boolean getLed (uint8_t led);

		void beep (uint16_t ms);

		void display (uint16_t val);
		void display (uint16_t val, boolean leading_zero);
		void showDisplay (boolean visible);
		void alarm (uint16_t freq, uint16_t period);
		void alarm (uint16_t freq);

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

		void displaySegment (uint8_t seg, uint8_t digit);

		static const uint8_t LED_PIN [];
		static const uint8_t KEY_PIN [];
		static const uint8_t SEG_MAP [];
		static const uint8_t SEG_SEL [];
};

static const uint8_t MFShield::LED_PIN [] = {13, 12, 11, 10};
static const uint8_t MFShield::KEY_PIN [] = {A1, A2, A3};
static const uint8_t MFShield::SEG_MAP [] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80 ,0x90};
static const uint8_t MFShield::SEG_SEL [] = {0xf1, 0xf2, 0xf4, 0xf8};


MFShield::MFShield ()
{
	for (uint8_t i=0; i < 4; i++)
	{
		pinMode (LED_PIN[i], OUTPUT);
		setLed (i+1, LOW);
		raw_segment[i] = 0xff;
	}
	for (uint8_t i=0; i < 3; i++)
		pinMode (KEY_PIN[i], INPUT_PULLUP);
	pinMode (MFS_PIN_TRIMMER, INPUT);
	pinMode (MFS_PIN_BUZZER, OUTPUT);
	pinMode (MFS_PIN_DISP_DO, OUTPUT);
	pinMode (MFS_PIN_DISP_CS, OUTPUT);
	pinMode (MFS_PIN_DISP_CK, OUTPUT);
	onKeyPress_func = NULL;

	t_disp = 0;
	t_buttons = 0;
	t_beep = 0;

	key_mask_pre = 0;
	disp_leading_zero = false;
	disp_enable = true;
	// showDisplay(false);
	beep (0);
}

void MFShield::displaySegment (uint8_t seg, uint8_t digit)
{
	digitalWrite(MFS_PIN_DISP_CS ,LOW);
	shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, (digit < 10) ? SEG_MAP[digit] : 0xff);
	shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, SEG_SEL[seg]);
	digitalWrite(MFS_PIN_DISP_CS ,HIGH);
}


void MFShield::display (uint16_t val, boolean leading_zero)
{
	raw_segment[0] = SEG_MAP [val / 1000];
	raw_segment[1] = SEG_MAP [(val / 100) % 10];
	raw_segment[2] = SEG_MAP [(val / 10) % 10];
	raw_segment[3] = SEG_MAP [val % 10];
	disp_enable = true;
	if (leading_zero)
		return;
	// else remove leading zeroes
	if (val < 1000)
		raw_segment [0] = 0xff;
	if (val < 100)
		raw_segment [1] = 0xff;
	if (val < 10)
		raw_segment [2] = 0xff;
}

void MFShield::display (uint16_t val)
{
	display (val, false);
}

uint16_t MFShield::getTrimmerValue ()
{
	return analogRead (MFS_PIN_TRIMMER);
}

void MFShield::loop ()
{
	uint32_t time = millis();
	// Update numeric display
	if (disp_enable && time - t_disp >= MFS_DISP_SCAN_INTERVAL_MS)
	{
		t_disp = time;
		current_segment = (current_segment > 3) ? 0 : current_segment + 1;
		digitalWrite(MFS_PIN_DISP_CS ,LOW);
		shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, raw_segment[current_segment]);
		shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, SEG_SEL[current_segment]);
		digitalWrite(MFS_PIN_DISP_CS ,HIGH);
	}
	// Keys polling
	if (onKeyPress_func != NULL && time - t_buttons >= MFS_BUTTONS_POLLING_INTERVAL_MS)
	{
		t_buttons = time;
		uint8_t key_mask = 0;
		for (uint8_t i=0; i < 3; i++)
			key_mask |= (!digitalRead(KEY_PIN[i]) << i);

		if (key_mask != key_mask_pre)
		{
			if (key_mask != 0)
			{
				key_mask_pre = key_mask;
				onKeyPress_func (ffs(key_mask));
			}
			else
				key_mask_pre = 0;
		}
	}

	if (t_beep != 0 && time > t_beep)
	{
			// turn off the buzzer
			digitalWrite (MFS_PIN_BUZZER, HIGH);
			t_beep = 0;
	}
}

void MFShield::showDisplay (boolean visible)
{
	if (!visible && disp_enable)
	{
		for (uint8_t i=0; i < 3; i++)
		{
			digitalWrite(MFS_PIN_DISP_CS ,LOW);
			shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, 0xff);
			shiftOut(MFS_PIN_DISP_DO, MFS_PIN_DISP_CK, MSBFIRST, SEG_SEL[i]);
			digitalWrite(MFS_PIN_DISP_CS ,HIGH);
		}
	}

	disp_enable = visible;
}

void MFShield::onKeyPress (void (*func)(uint8_t key))
{
	onKeyPress_func = func;
}

void MFShield::setLed (uint8_t led, boolean state)
{
	if (led < 1 || led > 4)
		return;
	digitalWrite (LED_PIN[led-1], !state);
}

boolean MFShield::getLed (uint8_t led)
{
	if (led < 1 || led > 4)
		return false;
	return !digitalRead (LED_PIN[led-1]);
}

void MFShield::beep (uint16_t ms)
{
	boolean turnoff = (ms == 0);
	digitalWrite (MFS_PIN_BUZZER, turnoff);
	t_beep = turnoff ? 0 : millis() + ms;
}


#endif