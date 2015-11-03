#include "accelerometer.h"

void initializeAccelerometer(AccelerometerDefinition *accelerometer) {
	P1OUT &= ~(BIT0 + BIT1 + BIT2);
	P1DIR &= ~(BIT0 + BIT1 + BIT2);

	// From p558 of MSP User guide:
	// "The DTC is enabled by setting the ADC10DTC1 register to a nonzero value."
	// ADC10DTC1 = ~0;

	ADC10CTL1 = INCH_2 + CONSEQ_3;            // A2/A1/A0, repeat multi channel
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
	ADC10AE0 = (BIT0 + BIT1 + BIT2);          // P1.0,1, 2 Analog enable
	ADC10DTC1 = 3;                         // number of conversions
}

void calibrateAccelerometer(AccelerometerDefinition *accelerometer) {

}

void updateAccelerometer(AccelerometerDefinition *accelerometer) {
	volatile unsigned int measurements[3];

	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY)
		;               // Wait if ADC10 core is active
	ADC10SA = (int) measurements;        // Data buffer start
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready
	__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	_NOP();                                 // space for debugger
	_NOP();                                // Set Breakpoint here to read ADC
	// ADCMEM?

	// TODO change to averaging code
		accelerometer->xVal = measurements[0];	// get fresh x, y, z vals
		accelerometer->yVal = measurements[1];
		accelerometer->zVal = measurements[2];
		filer(&accelerometer, accelerometer->xVal, accelerometer->yVal, accelerometer->zVal);	// send those to the circular buffer
}

void filter(AccelerometerDefinition *accelerometer, unsigned int xVal,unsigned int yVal, unsigned int zVal){
	volatile unsigned int buffer[3][8];
	accelerometer->xSum -= buffer[0][n_x];			// subtract old value from sums
	accelerometer->ySum -= buffer[1][n_x];
	accelerometer->zSum - buffer[2][n_x];

	buffer[0][n_x] = xVal;							// update current index with new values from the passed values
	buffer[1][n_x] = yVal;
	buffer[2][n_x] = zVal;

	accelerometer->xSum += buffer[0][n_x];			// add new values to the sum
	accelerometer->ySum += buffer[1][n_x];
	accelerometer->zSum += buffer[2][n_x];

	accelerometer->xAvg = accelerometer->xSum >> 3;	// bit shift by 3 to divide the x sum by 8 and update the struct average
	accelerometer->yAvg = accelerometer->ySum >> 3;
	accelerometer->zAvg = accelerometer->zSum >> 3;
	// now the update ADC averages are available within the average fields within the accelerometer struct
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
	__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
