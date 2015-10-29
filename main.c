// Ben Browning
// Will Cray

#include <msp430.h>
#include "accelerometer.h"
#include "LED.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    LEDRingDefinition ring;
	initializeLEDRing(&ring);
	animateLEDs(cycle);
	animateLEDs(cycle);
	animateLEDs(pie);

	// STARTUP STAGE
	// initialize LED ring
	// initialize accelerometer
	AccelerometerDefinition accelerometer;
	initializeAccelerometer(&accelerometer);

	// initialize PWM

	// CALIBRATION STAGE
	calibrateAccelerometer(&accelerometer);

	// NORMAL OPERATION -- LEVEL
	while (1)
	{
		// read ADC

		// CORDIC to figure out the LED to light

		// Update the LED ring
	}

	return 0;
}
