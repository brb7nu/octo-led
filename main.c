// Ben Browning
// Will Cray

#include <msp430.h>
#include "accelerometer.h"
#include "LED.h"
#include "timer.h"
#include "button.h"
#include "debounce.h"
#include "msp430io.h"

// #define LEFT_RED_TP XOUT // P1.0
// #define LEFT_WHITE_TP YOUT // P1.1
// #define LEFT_YELLOW_TP ZOUT // P1.2
// #define RIGHT_YELLOW_TP SIN
// #define RIGHT_WHITE_TP SCLK
// #define RIGHT_RED_TP XLATCH

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
				// read the 8-sample buffer for x, y, or z
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
