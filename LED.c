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

void updateLEDRing(LEDRingDefinition *ring, TimerDefinition *timer)
{
	// use ring->animation and timer to light the right LEDs

	// for each LEDLightDefinition in the ring, use the timer to determine whether it should be lit or dark
	int i;
	for (i = 0; i < 8; i++)
	{
		// ring->leds[i].
	}
}

void animateLEDs(LEDAnimation animation)
{
	switch (animation)
	{
		case pie:
			lightLEDs(N_LED);
			_delay_cycles(1600000);
			lightLEDs(NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(SE_LED | E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(S_LED | SE_LED | E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			lightLEDs(NW_LED | W_LED | SW_LED | S_LED | SE_LED | E_LED | NE_LED | N_LED);
			_delay_cycles(1600000);
			break;
		case cycle:
			lightLEDs(N_LED);
			_delay_cycles(1600000);
			lightLEDs(NE_LED);
			_delay_cycles(1600000);
			lightLEDs(E_LED);
			_delay_cycles(1600000);
			lightLEDs(SE_LED);
			_delay_cycles(1600000);
			lightLEDs(S_LED);
			_delay_cycles(1600000);
			lightLEDs(SW_LED);
			_delay_cycles(1600000);
			lightLEDs(W_LED);
			_delay_cycles(1600000);
			lightLEDs(NW_LED);
			_delay_cycles(1600000);
			break;
		default:
			break;
	}
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
