#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <msp430.h>
extern int n_x;						// index for circular buffer
extern volatile unsigned int buffer[3][8];

typedef struct
{
	int xOffset; // scaled value from -100 to 100
	int xMax; // max value, set by calibration
	int xMin; // min value, set by calibration
	int xVal; // most recent unscaled reading
	int xSum; // the sum of the current and past 7 readings
	int xAvg; // sum / 8, which is the same as sum >> 3

	int yOffset;
	int yMax;
	int yMin;
	int yVal;
	int ySum;
	int yAvg;

	int zOffset;
	int zMax;
	int zMin;
	int zVal;
	int zSum;
	int zAvg;
} AccelerometerDefinition;

void initializeAccelerometer(AccelerometerDefinition *accelerometer);
void calibrateAccelerometer(AccelerometerDefinition *accelerometer);
void updateAccelerometer(AccelerometerDefinition *accelerometer);
void filter(AccelerometerDefinition *accelerometer);

#endif
