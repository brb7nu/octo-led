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
	int xVal;
	int yVal;
	int zVal;
} AccelerometerDefinition;

void initializeAccelerometer(AccelerometerDefinition *accelerometer);
void calibrateAccelerometer(AccelerometerDefinition *accelerometer);
void updateAccelerometer(AccelerometerDefinition *accelerometer);

#endif
