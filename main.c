// Ben Browning
// Will Cray

#include <msp430.h>
#include "accelerometer.h"
#include "LED.h"
#include "timer.h"
#include "button.h"
#include "debounce.h"
#include "msp430io.h"

typedef enum {initialize, calibrationIndicate, calibrationMeasure, calibrationStore, levelReadADC, levelLighLEDs, levelCORDIC} SystemState;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	SystemState systemState = initialize;

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
	animateLEDs(pie); // TODO Do these using timers instead?

	_BIS_SR(GIE);

	while (1)
	{
		updateTimer(&timer);
		updateButtonState(&pushButton, &timer);
		updateLEDRing(&ring, &timer);
		updateAccelerometer(&accelerometer);

		switch (systemState)
		{
			case initialize:
				systemState = calibrationIndicate;
				break;
			case calibrationIndicate:
				// flash all LEDs to tell user to calibrate
				ring.animation = flash;

				// exit conditions for this state
				if (pushButton.state == pressed)
				{
					// calibrate this axis
					systemState = calibrationMeasure;
				}
				break;
			case calibrationMeasure:
				// take 8 samples
				ring.animation = cycle;
				systemState = calibrationIndicate;
				break;
			case calibrationStore:
				break;
			case levelReadADC:
				break;
			case levelLighLEDs:
				break;
			case levelCORDIC:
				break;
			default:
				break;
		}
	}

	return 0;
}
