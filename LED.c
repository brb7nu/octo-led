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

	ring->leds[lowerNeighbor].eventTime = timer->microseconds;
	ring->leds[lowerNeighbor].onTime = 500;
	ring->leds[ledNumber].eventTime = timer->microseconds;
	ring->leds[ledNumber].onTime = 1000;
	ring->leds[higherNeighbor].eventTime = timer->microseconds;
	ring->leds[higherNeighbor].onTime = 500;
}

void updateLEDRing(LEDRingDefinition *ring, TimerDefinition *timer)
{
	// use ring->animation and timer to light the right LEDs

	// for each LEDLightDefinition in the ring, use the timer to determine whether it should be lit or dark
	unsigned char mask;
	int i;
	for (i = 0; i < 8; i++)
	{
		if (timer->microseconds - ring->leds[i].eventTime > ring->leds[i].onTime)
		{
			// turn it off
		}
		else
		{
			// turn it off
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
