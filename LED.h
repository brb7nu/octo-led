#ifndef LED_H_
#define LED_H_

#include <msp430.h>
#include "timer.h"

#define SCK BIT5
#define BLANK BIT4
#define LATCH BIT0
#define SI BIT7

#define N_LED BIT7
#define NE_LED BIT6
#define E_LED BIT5
#define SE_LED BIT4
#define S_LED BIT3
#define SW_LED BIT2
#define W_LED BIT1
#define NW_LED BIT0

#define DUTY_CYCLE_BRIGHTEST 50
#define DUTY_CYCLE_BRIGHT 10
#define DUTY_CYCLE_DIM 2
#define DUTY_CYCLE_DIMMEST 1

typedef struct
{
	char mask;

	// stores the number of milliseconds each led must stay on
	unsigned int dutyCycleRemaining[8];

	// stores the high times in milliseconds for each LED in the ring
	// this is different than ledRingHighTimeRemaining because it does not get decremented
	unsigned int dutyCycle[8];
	unsigned int dutyIndex;
} LEDRingDefinition;

void lightLEDAndNeighbors(LEDRingDefinition *ring, char mask);
void updateLEDRing(LEDRingDefinition *ring);
void initializeLEDRing(LEDRingDefinition *ring);
void lightLEDMask(LEDRingDefinition *ring, char mask);
inline void reloadPWMTimes(LEDRingDefinition *ring);
inline void clearDutyCycles(LEDRingDefinition *ring);
inline void sendLEDMask(unsigned char mask);

#endif /* LED_H_ */
