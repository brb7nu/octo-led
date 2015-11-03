#include "LED.h"

void lightLEDAndNeighbors(LEDRingDefinition *ring, int ledNumber, TimerDefinition *timer)
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

	ring->leds[lowerNeighbor].onTimeRemaining = 500;
	ring->leds[ledNumber].onTimeRemaining = 1000;
	ring->leds[higherNeighbor].onTimeRemaining = 500;
}

void updateLEDRing(LEDRingDefinition *ring, TimerDefinition *timer)
{
	// for each LEDLightDefinition in the ring, use the timer to determine whether it should be lit or dark
	int i;
	for (i = 0; i < 8; i++)
	{
		if (ring->leds[i].onTimeRemaining > 0)
		{
			ring->leds[i].onTimeRemaining--;
		}
		else
		{
			// turn it off
			ring->ringMask &= ~(1 << i);
		}
	}

	sendLEDMask(ring->ringMask);
}

void sendLEDMask(unsigned char mask){
	send(mask);
	enableLatch();
	disableLatch();
}

void initializeLEDRing(LEDRingDefinition *ring){
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	P1DIR |= N_LED;
	P1DIR |= NE_LED;
	P1DIR |= E_LED;
	P1DIR |= SE_LED;
	P1DIR |= S_LED;
	P1DIR |= SW_LED;
	P1DIR |= W_LED;
	P1DIR |= NW_LED;

	int i;
	for (i = 0; i < 8; i++)
	{
		ring->leds[i].onTimeRemaining = 500;
	}

	ring->ringMask = 0x00;
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
