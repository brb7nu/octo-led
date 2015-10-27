// Will Cray
// Ben Browning

#include <msp430.h>

typedef enum {startup, calibration, level} SystemState;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	// STARTUP
	SystemState sysState = startup;

	// initialize LED ring
	// initialize accelerometer
	// initialize PWM

	while (1)
	{
		// state machine for entire system
		switch (sysState)
		{
			case calibration:
				break;
			case level:
				break;
		}
	}

	return 0;
}
