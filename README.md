# Multi Function Shield Library
MFShield is a small and easy library for a common cheap Arduino Multi-Function Shield<br />
Eliminates the need for using code snippets and pin definitions - you just simply call the library functions.

![picture](https://s-media-cache-ak0.pinimg.com/originals/f7/1d/6a/f71d6a26fb99f469412439486c58a147.jpg)
## What is does
- Calls a function when a button is pressed: <code>onKeyPress(yourFunction (uint8_t button_number))</code>
- Shows a number on the display: <code>display (int value)</code>
- Reads the analog value of potentiometer: <code>int readTrimmerValue ()</code> 
- Emits a sound with buzzer for a given duration<code>beep (int ms)</code>

## Quick start
1. Download and unpack a ZIP archive to your Arduino/libraries folder, rename folder MFShield-master to MFShield.
2. Open Arduino IDE and create a new sketch 
3. Include the library in your sketch by <code>#include &lt;MFShield.h&gt;</code>
4. Create an instance of class MFShield: <code>MFShield mfs;</code>
5. Add a <code>mfs.loop()</code> function in the main loop.

<details>
<summary><b>Basic example</b> <i>(click to view)</i></summary>

 ```arduino

#include <MFShield.h>

MFShield mfs;

void setup ()
{
	Serial.begin (9600);
	/* Handle buttons */
	mfs.onKeyPress ([](uint8_t button) {
		mfs.beep(10);	// make sound
		mfs.setLed (button, !mfs.getLed(button)); // toggle onboard leds
		Serial.println ("Button pressed: " + String(button));
	});
}

void loop ()
{
	/* Display seconds */
	mfs.display (millis() / 1000);

	mfs.loop();
	/* ^ It's important to insert this loop function in the main loop
	 *   else numeric display wont work
	 */
}
```
</details>

<b>Important</b>: avoid to use <code>delay()</code> and other blocking function in your sketch,
as it will prevent the library from updating the display and polling the buttons.
A much better practice would be to use <code>millis()</code> instead.

See examples for detailed description.
	
## TODO
	- Display negative and floating point numbers

