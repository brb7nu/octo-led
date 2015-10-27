#include "accelerometer.h"

void initializeAccelerometer(AccelerometerDefinition *myAccelerometer)
{
	// From p558 of MSP User guide:
	// "The DTC is enabled by setting the ADC10DTC1 register to a nonzero value."
	ADC10DTC1 = ~0;
}

void calibrateAccelerometer(AccelerometerDefinition *myAccelerometer)
{

}