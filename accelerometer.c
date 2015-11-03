#include "accelerometer.h"

void initializeAccelerometer(AccelerometerDefinition *accelerometer)
{
	P1OUT &= ~(BIT0 + BIT1 + BIT2);
	P1DIR &= ~(BIT0 + BIT1 + BIT2);

	// From p558 of MSP User guide:
	// "The DTC is enabled by setting the ADC10DTC1 register to a nonzero value."
	ADC10DTC1 = ~0;
}

void calibrateAccelerometer(AccelerometerDefinition *accelerometer)
{

}

void updateAccelerometer(AccelerometerDefinition *accelerometer)
{

}
