#include "LED.h"

void lightLEDAndNeighbors(LEDRingDefinition *ring, char mask)
{
	int ledNumber;
	while (mask)
	{
		mask >>= 1;
		ledNumber += 1;	
	}

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

	clearDutyCycles(ring);

	ring->dutyCycle[lowerNeighbor] = DUTY_CYCLE_DIM;
	ring->dutyCycle[ledNumber] = DUTY_CYCLE_BRIGHTEST;
	ring->dutyCycle[higherNeighbor] = DUTY_CYCLE_DIM;
}

void clearDutyCycles(LEDRingDefinition *ring)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		ring->dutyCycle[i] = 0;
	}
}

void lightLEDMask(LEDRingDefinition *ring, char mask)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (mask >> i && 0x1) // from LSB to MSB
		{
			ring->dutyCycle[i] = 100;
		}
		else
		{
			ring->dutyCycle[i] = 0;
		}
	}
	//_delay_cycles(1000000);
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

inline void sendLEDMask(unsigned char mask){
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

	clearDutyCycles(ring);
	
	int i;
	for (i = 0; i < 8; i++)
	{
		ring->dutyCycleRemaining[i] = 0;
	}
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

inline void enableLatch(){
	P2OUT |= LATCH;
}

inline void disableLatch(){
	P2OUT &= ~LATCH;
}

inline void enableBlank(){
	P1OUT |= BLANK;
}

inline void disableBlank(){
	P1OUT &= ~BLANK;
}

inline void pulseClock(){
	 P1OUT |= SCK;
	 P1OUT &= ~SCK;
}
