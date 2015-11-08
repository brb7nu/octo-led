#ifndef LED_H_
#define LED_H_

#include <msp430.h>
#include "timer.h"

#define SCK BIT5
#define BLANK BIT4
#define LATCH BIT0
#define SI BIT7

#define N_LED 7
#define NE_LED 6
#define E_LED 5
#define SE_LED 4
#define S_LED 3
#define SW_LED 2
#define W_LED 1
#define NW_LED 0

#define DUTY_CYCLE_BRIGHTEST 100
#define DUTY_CYCLE_BRIGHT 20
#define DUTY_CYCLE_DIM 5
#define DUTY_CYCLE_DIMMEST 1

typedef struct
{
	char mask;

	// stores the number of milliseconds each led must stay on
	int dutyCycleRemaining[8];

	// stores the high times in milliseconds for each LED in the ring
	// this is different than ledRingHighTimeRemaining because it does not get decremented
	int dutyCycle[8];
	int dutyIndex;
} LEDRingDefinition;

void lightLEDAndNeighbors(LEDRingDefinition *ring, int ledNumber, TimerDefinition *timer);
void updateLEDRing(LEDRingDefinition *ring);
void initializeLEDRing(LEDRingDefinition *ring);
void lightOneLED(LEDRingDefinition *ring, char ledNumber);
void reloadPWMTimes(LEDRingDefinition *ring);

// private SPI communication functions
void send(unsigned char s);
void enableLatch();                         // Allows operations to be carried out on LED Driver
void disableLatch();                        // Prevents operations from being carried out on LED Driver
void enableBlank();                         //
void disableBlank();                        //
void pulseClock();                          // Causes a clock tick to be sent to LED Driver

#endif /* LED_H_ */
