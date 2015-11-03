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
	sendLEDMask(N_LED);
	_delay_cycles(800000);
	sendLEDMask(NE_LED);
	_delay_cycles(800000);
	sendLEDMask(E_LED);
	_delay_cycles(800000);
	sendLEDMask(SE_LED);
	_delay_cycles(800000);
	sendLEDMask(S_LED);
	_delay_cycles(800000);
	sendLEDMask(SW_LED);
	_delay_cycles(800000);
	sendLEDMask(W_LED);
	_delay_cycles(800000);
	sendLEDMask(NW_LED);
	_delay_cycles(800000);
	sendLEDMask(N_LED);
	_delay_cycles(800000);
	sendLEDMask(NE_LED);
	_delay_cycles(800000);
	sendLEDMask(E_LED);
	_delay_cycles(800000);
	sendLEDMask(SE_LED);
	_delay_cycles(800000);
	sendLEDMask(S_LED);
	_delay_cycles(800000);
	sendLEDMask(SW_LED);
	_delay_cycles(800000);
	sendLEDMask(W_LED);
	_delay_cycles(800000);
	sendLEDMask(NW_LED);
	_delay_cycles(800000);
	sendLEDMask(N_LED);
	_delay_cycles(800000);
	sendLEDMask(NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(E_LED | NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(SE_LED | E_LED | NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(S_LED | SE_LED | E_LED | NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
	_delay_cycles(800000);
	sendLEDMask(NW_LED | W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
	_delay_cycles(800000);

	_BIS_SR(GIE);

	while (1)
	{
		updateTimer(&timer);
		updateButtonState(&pushButton, &timer);
		// updateAccelerometer(&accelerometer);
		updateLEDRing(&ring, &timer);

		switch (systemState)
		{
			case initialize:
				systemState = calibrationIndicate;
				break;
			case calibrationIndicate:
				// flash all LEDs to tell user to calibrate


				// exit conditions for this state
				if (pushButton.state == pressed)
				{
					// calibrate this axis
					systemState = calibrationMeasure;
				}
				break;
			case calibrationMeasure:
				// read the 8-sample buffer for x, y, or z
				P1OUT |= BIT6;
				_delay_cycles(6400000);
				P1OUT &= ~BIT6;
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
