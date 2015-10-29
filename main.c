// Ben Browning
// Will Cray

#include <msp430.h>
#include "accelerometer.h"
#include "LED.h"
#include "timer.h"
#include "button.h"
#include "debounce.h"
#include "msp430io.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // ---------------- INITIALIZE STATE ----------------
    // initialize timer
    TimerDefinition timer;
    initializeTimer(&timer);

	// initialize LED ring
    LEDRingDefinition ring;
	initializeLEDRing(&ring);

	// initialize accelerometer
	AccelerometerDefinition accelerometer;
	initializeAccelerometer(&accelerometer);

	// initialize button
	ButtonDefinition pushButton;
	initializeMSP430IO(&pushButton);

	// initialize PWM

	// Animate LEDs to indicate that initialization state is completed
	animateLEDs(cycle);
	animateLEDs(cycle);
	animateLEDs(pie);

	// ---------------- INDICATE STATE ----------------
	// flash all LEDs to tell user to calibrate

	// ---------------- WAITBUTTON STATE ----------------
	int i = 0;
	while (accelerometer.isCalibrated)
	{
		updateTimer(&timer);
		updateButtonState(&pushButton, &timer);
		if (pushButton.state == pressed)
		{
			// calibrate this axis
			calibrateAccelerometerAxis(&accelerometer, );
			i++;
		}

		// TODO Need an exit condition
	}

	// NORMAL OPERATION -- LEVEL
	while (1)
	{
		// read ADC

		// CORDIC to figure out the LED to light

		// Update the LED ring
	}

	return 0;
}
