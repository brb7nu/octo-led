// Ben Browning
// Will Cray

#include <msp430.h>
#include "accelerometer.h"
#include "LED.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	initializeLEDPorts();
	initializeLEDPins();			// intialize the LEDs
	initializeLEDS();
	
	// STARTUP
	// initialize LED ring
	// initialize accelerometer
	// initialize PWM

	// CALIBRATION


	// NORMAL OPERATION -- LEVEL
	while (1)
	{

	}

	

	return 0;
}
