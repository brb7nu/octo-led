#include <msp430.h> 
#include "LED.h"
/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	initializeLEDPorts();
	initializeLEDPins();			// intialize the LEDs
	initializeLEDS();


	while (1) {


	}

return 0;
}
