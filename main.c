// Ben Browning
// Will Cray

#include <msp430.h>
#include "msp430io.h"
#include "button.h"
#include "timer.h"
#include "led.h"
#include "accelerometer.h"

// #define LEFT_RED_TP XOUT // P1.0
// #define LEFT_WHITE_TP YOUT // P1.1
// #define LEFT_YELLOW_TP ZOUT // P1.2
// #define RIGHT_YELLOW_TP SIN
// #define RIGHT_WHITE_TP SCLK
// #define RIGHT_RED_TP XLATCH

#define SCK BIT5
#define BLANK BIT4
#define LATCH BIT0
#define SI BIT7

TimerDefinition timer;
LEDRingDefinition ring;
ButtonDefinition pushButton;
AccelerometerDefinition accelerometer;

// NO need for LED mask because it just holds the non-zero values in ledRingHighTimeRemaining

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	initializeTimer(&timer);
	initializeMSP430IO(&pushButton);
	initializeLEDRing(&ring);
	// initializeAccelerometer(&accelerometer);

	_BIS_SR(GIE);

	while (1)
	{
		updateTimer(&timer);
		updateButtonState(&pushButton, &timer);
		// updateAccelerometer(&accelerometer);
	}

	return 0;
}

//  ___         ___         ___        
// |   |_______|   |_______|   |_______
//  _______     _______     _______    
// |       |___|       |___|       |___
//  ___         ___         ___        
// |   |_______|   |_______|   |_______
//
// ____________________________________
// 
// |---10 ms---|
#pragma vector = TIMER0_A0_VECTOR // Timer A interrupt service routine
__interrupt void TimerA0_routine(void)
{
	g100uSTimeout++;
	updateLEDRing(&ring);
}

