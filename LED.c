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

void updateLEDRing(LEDRingDefinition *ring)
{
	// for each LEDLightDefinition in the ring, use the timer to determine whether it should be lit or dark
	if (ring->dutyIndex >= 100)
	{
		ring->dutyIndex = 0;
		reloadPWMTimes(ring);
		P1OUT ^= BIT6;
	}

	ring->mask = 0x00;

	// send a mask based on the LEDs that still have high time remaining
	int i;
	for (i = 0; i < 8; i++)
	{
		if (ring->dutyCycleRemaining[i])
		{
			ring->mask |= (0x01 << i);
			ring->dutyCycleRemaining[i]--;
		}
	}

	sendLEDMask(ring->mask);

	ring->dutyIndex++;
}

void sendLEDMask(unsigned char mask){
	send(mask);
	enableLatch();
	disableLatch();
}

void reloadPWMTimes(LEDRingDefinition *ring)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		ring->dutyCycleRemaining[i] = ring->dutyCycle[i];
	}
}

void initializeLEDRing(LEDRingDefinition *ring)
{
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	ring->mask = 0x00;
	ring->dutyIndex = 0;
	
	int i;
	for (i = 0; i < 8; i++)
	{
		ring->dutyCycleRemaining[i] = 0;
	}

	ring->dutyCycle[5] = DUTY_CYCLE_DIM;
	ring->dutyCycle[6] = DUTY_CYCLE_BRIGHTEST;
	ring->dutyCycle[7] = DUTY_CYCLE_DIM;
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