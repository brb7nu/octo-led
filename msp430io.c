#include "msp430io.h"

#define BUTTON_PIN BIT3
#define BUTTON_PORT ((char*) &(P1IN))

#define BUTTON_PRESS_DEBOUNCE_TIME_MS 10
#define BUTTON_RELEASE_DEBOUNCE_TIME_MS 20

void initializeMSP430IO(ButtonDefinition *pushButton)
{
	// set up LED ports
	// P1OUT &= ~(BIT0 + BIT6);
	// P1DIR |= (BIT0 + BIT6);

	// set up button (pin 1.3) for input
	P1DIR &= ~BIT3;
	
	// set pull up/down resistor to pull up
	P1OUT |= BIT3;

	// enable pull up/down resistor
	P1REN |= BIT3;

	pushButton->buttonSignal.controlState = dbExpectHigh;
	pushButton->buttonSignal.signalPort = BUTTON_PORT;
	pushButton->buttonSignal.signalPin = BUTTON_PIN;
	pushButton->buttonSignal.validateHighTime = BUTTON_RELEASE_DEBOUNCE_TIME_MS;
	pushButton->buttonSignal.validateLowTime = BUTTON_PRESS_DEBOUNCE_TIME_MS;
	pushButton->buttonSignal.validatedSignalValue = high;
	pushButton->state = unpressed;
	pushButton->count = 0;
}
