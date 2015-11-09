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

	// when cycleIndex == offTick[i], turn that LED off
	unsigned int offTick[8]; // ranges from [0, 99]
	unsigned int cycleIndex;
} LEDRingDefinition;

void setLEDAndNeighbors(LEDRingDefinition *ring, char mask);
void adjustPWM(LEDRingDefinition *ring);
void initializeLEDRing(LEDRingDefinition *ring);
void lightLEDMask(LEDRingDefinition *ring, char mask);
inline void clearDutyCycles(LEDRingDefinition *ring);
inline void sendLEDMask(unsigned char mask);

#endif /* LED_H_ */
