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
	char mask;

	// stores the number of milliseconds each led must stay on
	int dutyCycleRemaining[8];

	// stores the high times in milliseconds for each LED in the ring
	// this is different than ledRingHighTimeRemaining because it does not get decremented
	int dutyCycle[8];
	int dutyIndex;
} LEDRingDefinition;

void lightLEDAndNeighbors(LEDRingDefinition *ring, char mask);
void updateLEDRing(LEDRingDefinition *ring);
void initializeLEDRing(LEDRingDefinition *ring);
void lightLEDMask(LEDRingDefinition *ring, char mask);
void reloadPWMTimes(LEDRingDefinition *ring);
void clearDutyCycles(LEDRingDefinition *ring);
inline void sendLEDMask(unsigned char mask);

// private SPI communication functions
void send(unsigned char s);
inline void enableLatch();                         // Allows operations to be carried out on LED Driver
inline void disableLatch();                        // Prevents operations from being carried out on LED Driver
inline void enableBlank();                         //
inline void disableBlank();                        //
inline void pulseClock();                          // Causes a clock tick to be sent to LED Driver

#endif /* LED_H_ */
