// Will Cray
// Ben Browning

#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    // STARTUP
    // initialize LED ring
    // initialize accelerometer
    // initialize PWM

    // CALIBRATION
    while () // while device is uncalibrated
    {

    }

    // NORMAL OPERATION -- LEVEL
    while (1)
    {

    }

	return 0;
}
