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

	ring->leds[lowerNeighbor].eventTime = timer->milliseconds;
	ring->leds[lowerNeighbor].onTimeMS = 2;
	ring->leds[ledNumber].eventTime = timer->milliseconds;
	ring->leds[ledNumber].onTimeMS = 4;
	ring->leds[higherNeighbor].eventTime = timer->milliseconds;
	ring->leds[higherNeighbor].onTimeMS = 2;
}

void updateLEDRing(LEDRingDefinition *ring, TimerDefinition *timer)
{
	// use ring->animation and timer to light the right LEDs

	// for each LEDLightDefinition in the ring, use the timer to determine whether it should be lit or dark
	unsigned char mask;
	int i;
	for (i = 0; i < 8; i++)
	{
		if (timer->milliseconds - ring->leds[i].eventTime > ring->leds[i].onTimeMS)
		{
			// turn it off
		}
		else
		{
			mask += 1 << i;
		}
	}

	lightLEDs(mask);
}

void lightLEDs(unsigned char mask){
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
