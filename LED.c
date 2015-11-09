#include "LED.h"

void lightLEDAndNeighbors(LEDRingDefinition *ring, char mask)
{
	unsigned int ledNumber = 0;
	while (mask)
	{
		mask >>= 1;
		ledNumber += 1;	
	}

	unsigned int lowerNeighbor = ledNumber - 1;
	unsigned int higherNeighbor = ledNumber + 1;

	if (ledNumber == 0)
	{
		lowerNeighbor = 7;
	}
	else if (ledNumber == 7)
	{
		higherNeighbor = 0;
	}

	clearDutyCycles(ring);

	ring->dutyCycle[lowerNeighbor] = DUTY_CYCLE_DIM;
	ring->dutyCycle[ledNumber] = DUTY_CYCLE_BRIGHTEST;
	ring->dutyCycle[higherNeighbor] = DUTY_CYCLE_DIM;
}

inline void clearDutyCycles(LEDRingDefinition *ring)
{
	unsigned int i = 8;
	while (i)
	{
		i--;
		ring->dutyCycle[i] = 0;
	}
}

void lightLEDMask(LEDRingDefinition *ring, char mask)
{
	unsigned int i = 8;
	while (i)
	{
		i--;
		if ((mask >> i) & 0x1) // from MSB to LSB
		{
			ring->dutyCycle[i] = 100;
		}
		else
		{
			ring->dutyCycle[i] = 0;
		}
	}
}

void updateLEDRing(LEDRingDefinition *ring)
{
	if (ring->dutyIndex == 0)
	{
		ring->dutyIndex = 100;
		unsigned int i = 8;
		while (i)
		{
			i--;
			ring->dutyCycleRemaining[i] = ring->dutyCycle[i];
		}		
		P1OUT ^= BIT6;
	}

	ring->mask = 0x00;

	// send a mask based on the LEDs that still have high time remaining
	unsigned int i = 8;
	while (i)
	{
		i--;
		if (ring->dutyCycleRemaining[i])
		{
			ring->mask |= (0x01 << i);
			ring->dutyCycleRemaining[i]--;
		}
	}

	sendLEDMask(ring->mask);

	ring->dutyIndex--;
}

inline void sendLEDMask(unsigned char mask){
    unsigned char bit = 0x80;

    while(bit){
        if( mask & bit ){
            P1OUT |= SI;
        }
        else{
            P1OUT &= ~SI;
        }
        bit >>= 1;
        P1OUT |= SCK;
		P1OUT &= ~SCK;
    }
	P2OUT |= LATCH;
	P2OUT &= ~LATCH;
}

void initializeLEDRing(LEDRingDefinition *ring)
{
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	ring->mask = 0x00;
	ring->dutyIndex = 0;

	clearDutyCycles(ring);
	
	unsigned int i = 8;
	while (i)
	{
		i--;
		ring->dutyCycleRemaining[i] = 0;
	}
}
