#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <msp430.h>

typedef struct
{
	int xMax;
	int xMin;
	int yMax;
	int yMin;
	int zMax;
	int zMin;
} AccelerometerDefinition;

void initializeAccelerometer(AccelerometerDefinition *myAccelerometer);
void calibrateAccelerometer(AccelerometerDefinition *myAccelerometer);

#endif
