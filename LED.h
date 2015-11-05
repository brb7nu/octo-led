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

#define DUTY_CYCLE_BRIGHTEST 100
#define DUTY_CYCLE_BRIGHT 20
#define DUTY_CYCLE_DIM 5
#define DUTY_CYCLE_DIMMEST 1

typedef struct
{
	int onTimeRemaining;
	// TODO add pin and port
} LEDLightDefinition;

typedef struct
{
	LEDLightDefinition leds[8];
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
void lightLEDs(unsigned char mask);

// private SPI communication functions
void send(unsigned char s);
void enableLatch();                         // Allows operations to be carried out on LED Driver
void disableLatch();                        // Prevents operations from being carried out on LED Driver
void enableBlank();                         //
void disableBlank();                        //
void pulseClock();                          // Causes a clock tick to be sent to LED Driver

#endif /* LED_H_ */
