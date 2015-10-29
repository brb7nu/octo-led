/*
 * LED.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Student
 */
#ifndef LED_H_
#define LED_H_

#include <msp430.h>

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

typedef struct
{
	int dutyCycle; // TODO maybe change to on ticks?
	int onTimeRemaining;
} LEDLightDefinition;

typedef struct
{
	LEDLightDefinition leds[8];
} LEDRingDefinition;

void lightLEDAndNeighbors(LEDRingDefinition *ring, int ledNumber);
void allLEDsOff(LEDRingDefinition *ring);
void updateRing(LEDRingDefinition *ring);

void initializeLEDS();
void lightLED(unsigned char LED);

void send(unsigned char s);

void initializeLEDPins();
void initializeLEDPorts();

void enableLatch();                         // Allows operations to be carried out on LED Driver
void disableLatch();                        // Prevents operations from being carried out on LED Driver

void enableBlank();                         //
void disableBlank();                        //

void pulseClock();                          // Causes a clock tick to be sent to LED Driver



#endif /* LED_H_ */
