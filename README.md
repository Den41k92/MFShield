# Multi Function Shield
MFShield is a small and easy library for a common cheap Arduino Multi-Function Shield

## What is does
- Calls a function when a button is pressed: onKeyPress(yourFunction)
- Shows a number on the display: display(value)
- Reads 

## To begin
1) Include the library in your sketch by	#include <MFShield.h>
2) Create an object of class MFShield:		MFShield mfs;
3) Add a mfs.loop() function in the main loop:
	void loop ()
	{
		mfs.loop();
		// your code here
	}
[!] Important: avoid to use delay() and other blocking function in your sketch,
as it will prevent the library from updating the display and polling the buttons.
A much better practice would be to use millis() instead.

See examples for detailed description.
	
## TODO
	- Display negative and floating point numbers
	- Binary level optimisation (use port registers instead of digitalWrite())