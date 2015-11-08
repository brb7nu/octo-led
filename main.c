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

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	initializeTimer(&timer);
	initializeMSP430IO(&pushButton);
	initializeLEDRing(&ring);
	initializeAccelerometer(&accelerometer);

	_BIS_SR(GIE);

	////BEGIN INITIALIZATION/////
	// xMax
	while(1){
		lightOneLED(&ring, N_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.xMax = accelerometer.xAvg;
			break;
		}
	}
	// xMin
	while(1){
		lightOneLED(&ring, S_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.xMin = accelerometer.xAvg;
			break;
		}
	}
	// yMax
	while(1){
		lightOneLED(&ring, W_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.yMax = accelerometer.yAvg;
			break;
		}
	}
	// yMin
	while(1){
		lightOneLED(&ring, E_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.yMin = accelerometer.yAvg;
			break;
		}
	}
	// zMax
	while(1){
		lightOneLED(&ring, N_LED);
		lightOneLED(&ring, SE_LED);
		lightOneLED(&ring, SW_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.zMax = accelerometer.zAvg;
			break;
		}
	}
	// zMin
	while(1){
		lightOneLED(&ring, S_LED);
		lightOneLED(&ring, NW_LED);
		lightOneLED(&ring, NE_LED);
		updateTimer(&timer);
		ButtonState previous = pushButton.state;
		updateButtonState(&pushButton, &timer);
		if (previous == unpressed && pushButton.state == pressed){
			accelerometer.zMax = accelerometer.zAvg;
			break;
		}
	}

	while (1)
	{
		updateTimer(&timer);
		updateButtonState(&pushButton, &timer);
		updateAccelerometer(&accelerometer);

		coord.x = accelerometer.xOffset;
		coord.y = accelerometer.yOffset;
		coord.z = accelerometer.zOffset;
		meas = getDisplaySetting(&coord);

		//lightLEDAndNeighbors(&ring, 3, &timer);

		// switch (systemState)
		// {
		// 	case initialize:
		// 		systemState = calibrationIndicate;



		// 		break;
		// 	case calibrationIndicate:
		// 		// flash all LEDs to tell user to calibrate

		// 		// exit conditions for this state
		// 		if (pushButton.state == pressed)
		// 		{
		// 			// calibrate this axis
		// 			systemState = calibrationMeasure;
		// 		}
		// 		break;
		// 	case calibrationMeasure:
		// 		// read the 8-sample buffer for x, y, or z
		// 		systemState = calibrationIndicate;
		// 		break;
		// 	case calibrationStore:
		// 		break;
		// 	case levelReadADC:
		// 		break;
		// 	case levelLighLEDs:
		// 		break;
		// 	case levelCORDIC:
		// 		break;
		// 	default:
		// 		break;
		// }
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

