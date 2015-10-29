/*
 * LED.c
 *
 *  Created on: Oct 27, 2015
 *      Author: Student
 */
#include <msp430.h>
#include "LED.h"

void lightLEDAndNeighbors(LEDRingDefinition *ring, int ledNumber)
{
	int lowerNeighbor = ledNumber - 1;
	int higherNeighbor = ledNumber + 1;

	if (ledNumber == 0)
	{
		lowerNeighbor = 7;
	}
	else if (ledNumber == 7)
	{
		higherNeighbor = 0;
	}

	// clear the current PWM settings
	allLEDsOff(ring);

	ring->leds[lowerNeighbor].dutyCycle = 30;
	ring->leds[ledNumber].dutyCycle = 100;
	ring->leds[higherNeighbor].dutyCycle = 30;	
}

void allLEDsOff(LEDRingDefinition *ring)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		ring->leds[i].dutyCycle = 0;
	}
}

// Based on each 
void updateRing(LEDRingDefinition *ring)
{
	// get downmost LED using CORDIC. TODO Or maybe do this outside of the led code, in main function?

	// determine which LEDs to light based on their dutyCycles
	int i;
	for (i = 0; i < 8; i++)
	{
		// if (ring->leds[i].onTimeRemaining)
	}
}

void initializeLEDS(){
	int i = 0;
	
	for(i = 0; i<2; i++){
		lightLED(N_LED);
		lightLED(NE_LED);
		lightLED(E_LED);
		lightLED(SE_LED);
		lightLED(S_LED);
		lightLED(SW_LED);
		lightLED(W_LED);
		lightLED(NW_LED);
	}

	lightLED(N_LED);

	lightLED(NE_LED | N_LED);

	lightLED(E_LED | NE_LED | N_LED);

	lightLED(SE_LED | E_LED | NE_LED | N_LED);

	lightLED(S_LED | SE_LED | E_LED | NE_LED | N_LED);

	lightLED(SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);

	lightLED(W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);

	lightLED(NW_LED | W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);

}

void lightLED(unsigned char mask){
	send(mask);
	enableLatch();
	disableLatch();
	_delay_cycles(30000);
}

void initializeLEDPorts(){
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );
}

void initializeLEDPins(){
	P1DIR |= N_LED;
	P1DIR |= NE_LED;
	P1DIR |= E_LED;
	P1DIR |= SE_LED;
	P1DIR |= S_LED;
	P1DIR |= SW_LED;
	P1DIR |= W_LED;
	P1DIR |= NW_LED;
}

void send(unsigned char s){
    unsigned char bit = 0x80;

    while(bit){
        if( s & bit ){
            P1OUT |= SI;
        }
        else{
            P1OUT &= ~SI;
        }
        bit >>= 1;
        pulseClock();
    }
}

void enableLatch(){
	P2OUT |= LATCH;
}

void disableLatch(){
	P2OUT &= ~LATCH;
}

void enableBlank(){
	P1OUT |= BLANK;
}

void disableBlank(){
	P1OUT &= ~BLANK;
}

void pulseClock(){
	 P1OUT |= SCK;
	 P1OUT &= ~SCK;
}
