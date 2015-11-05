#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <msp430.h>
extern int n_x;						// index for circular buffer

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
	int xSum;
	int xAvg;
	int ySum;
	int yAvg;
	int zSum;
	int zAvg;
} AccelerometerDefinition;

void initializeAccelerometer(AccelerometerDefinition *accelerometer);
void calibrateAccelerometer(AccelerometerDefinition *accelerometer);
void updateAccelerometer(AccelerometerDefinition *accelerometer);
void filter(AccelerometerDefinition *accelerometer);

#endif
