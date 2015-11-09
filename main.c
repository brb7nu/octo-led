// Ben Browning
// Will Cray

#include <msp430.h>
#include "msp430io.h"
#include "button.h"
#include "timer.h"
#include "led.h"
#include "accelerometer.h"
#include "cordic.h"

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
coordinates coord;
measurements meas;

long phi;
long theta;
long XYhypot;
long Zhypot;

// NO need for LED mask because it just holds the non-zero values in ledRingHighTimeRemaining

// MAKE SURE TO DISCONNECT THE JUMPERS FOR RXD, TXD, AND GREEN LED

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	initializeTimer(&timer);
	initializeMSP430IO(&pushButton);
	initializeLEDRing(&ring);
	initializeAccelerometer(&accelerometer);

	_BIS_SR(GIE);

	int i;

	////BEGIN INITIALIZATION/////
	// xMax
	lightLEDMask(&ring, N_LED);
	waitUntil(&pushButton, pressed, &timer);

	// button is pressed, so calibrate xmax
	for (i = 0; i < 8; i++){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.xMax = accelerometer.xAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// xMin
	lightLEDMask(&ring, S_LED);
	waitUntil(&pushButton, pressed, &timer);
	for (i = 8; i >0; i--){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.xMin = accelerometer.xAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// yMax
	lightLEDMask(&ring, W_LED);
	waitUntil(&pushButton, pressed, &timer);
	for (i = 8; i >0; i--){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.yMax = accelerometer.yAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// yMin
	lightLEDMask(&ring, E_LED);
	waitUntil(&pushButton, pressed, &timer);
	for (i = 8; i >0; i--){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.yMin = accelerometer.yAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// zMax
	lightLEDMask(&ring, N_LED + SE_LED + SW_LED);
	waitUntil(&pushButton, pressed, &timer);
	for (i = 0; i < 8; i++){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.zMax = accelerometer.zAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// zMin
	lightLEDMask(&ring, S_LED + NW_LED + NE_LED);
	waitUntil(&pushButton, pressed, &timer);
	for (i = 0; i < 8; i++){
		updateAccelerometer(&accelerometer);
	}
	accelerometer.zMin = accelerometer.zAvg;
	waitUntil(&pushButton, unpressed, &timer);

	// DON'T CALL THIS METHOD TOO OFTEN
	setLEDAndNeighbors(&ring, BIT3);

	while (1)
	{
		updateTimer(&timer);
		updateAccelerometer(&accelerometer);

		coord.x = accelerometer.xNormalized;
		coord.y = accelerometer.yNormalized;
		coord.z = accelerometer.zNormalized;
		meas = getDisplaySetting(&coord);

		// cutoff angles are 1/16th offset from 3600 / 8
		if (2250 <= meas.angle && meas.angle < 6750)
		{
			setLEDAndNeighbors(&ring, BIT1);
		}
		else if (6750 <= meas.angle && meas.angle < 11250)
		{
			setLEDAndNeighbors(&ring, BIT2);
		}
		else if (11250 <= meas.angle && meas.angle < 15750)
		{
			setLEDAndNeighbors(&ring, BIT3);
		}
		else if (15750 <= meas.angle && meas.angle < 20250)
		{
			setLEDAndNeighbors(&ring, BIT4);
		}
		else if (20250 <= meas.angle && meas.angle < 24750)
		{
			setLEDAndNeighbors(&ring, BIT5);
		}
		else if (24750 <= meas.angle && meas.angle < 29250)
		{
			setLEDAndNeighbors(&ring, BIT6);
		}
		else if (29250 <= meas.angle && meas.angle < 33750)
		{
			setLEDAndNeighbors(&ring, BIT7);
		}
		else
		{
			setLEDAndNeighbors(&ring, BIT0);
		}
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
	timeout++;
	adjustPWM(&ring);
}

