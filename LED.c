#include "LED.h"

void setLEDAndNeighbors(LEDRingDefinition *ring, char mask)
{
	// BIT3 == 0b00001000
	// shoudl result in ledNumber 3

	// 2
	// mask = 0b00000100
	//

	unsigned int ledNumber = 0;
	while (mask)
	{
		mask >>= 1;
		ledNumber += 1;	
	}
	ledNumber--;

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

	ring->offTick[lowerNeighbor] = DUTY_CYCLE_DIM;
	ring->offTick[ledNumber] = DUTY_CYCLE_BRIGHTEST;
	ring->offTick[higherNeighbor] = DUTY_CYCLE_DIM;
}

inline void clearDutyCycles(LEDRingDefinition *ring)
{
	unsigned int i = 8;
	while (i)
	{
		i--;
		ring->offTick[i] = 0;
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
			ring->offTick[i] = 100;
		}
		else
		{
			ring->offTick[i] = 0;
		}
	}
}

void adjustPWM(LEDRingDefinition *ring)
{
	// if index has reached the turn off tick for any LED
	// in the ring, set it low
	ring->cycleIndex++;
	if (ring->cycleIndex == 100)
	{
		ring->cycleIndex = 0;
		ring->mask = 0xFF;
		P1OUT ^= BIT6;
	}

	int i;
	for (i = 0; i < 8; i++)
	{
		if (ring->cycleIndex == ring->offTick[i])
		{
			ring->mask &= ~(0x01 << i);
		}
	}

	sendLEDMask(ring->mask);
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

	ring->mask = 0xFF;
	ring->cycleIndex = 0;

	clearDutyCycles(ring);
	
	unsigned int i = 8;
	while (i)
	{
		i--;
		ring->offTick[i] = 0;
	}
}
