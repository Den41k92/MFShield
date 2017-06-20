#include <MFShield.h>

/* Create an object which is needed by library to control the shield */
	MFShield mfs;

/* This function is called every time any button is pressed */
/* The only argument means the number of the button pressed */
void showKey (uint8_t key)
{
	// print out the button number
	Serial.println("Button pressed: " + String (key, DEC));

	// turn on and off a led which has the number equal to the button's number
	mfs.setLed (key, !mfs.getLed (key));

	// make a short beep for 5 ms (because it's too loud)!
	mfs.beep (5);
}

void setup()
{
	Serial.begin(9600);

	// Assign the custom function 'showKey' (see above) to the button press event
	// Note: this function must have one argument (8-bit variable) which defines the button number is being pressed
	mfs.onKeyPress (showKey);
}

void loop()
{
	// Always insert mfs.loop() in the main loop, without this function the MFShield library wont work.
	// It's a neccessary routine needed to update the display, poll the buttons and run the internal timer.
	mfs.loop();

	static uint32_t t = millis();
	// Run this cycle once every 200 msec
	if (millis() - t >= 200)
	{
		t = millis();
		// Shows the potentiometer (trimmer) value on it's numeric display
		uint16_t trimmer = mfs.getTrimmerValue();
		mfs.display(trimmer);
	}
}
