// Ben Browning
// Will Cray

#include <msp430.h>
#include "timer.h"

// #define LEFT_RED_TP XOUT // P1.0
// #define LEFT_WHITE_TP YOUT // P1.1
// #define LEFT_YELLOW_TP ZOUT // P1.2
// #define RIGHT_YELLOW_TP SIN
// #define RIGHT_WHITE_TP SCLK
// #define RIGHT_RED_TP XLATCH

#define SCK BIT5
#define BLANK BIT4
#define LATCH BIT0
#define SI BIT7

// stores the number of milliseconds each led must stay on
int ledRing[8];

// NO need for LED mask because it just holds the non-zero values in ledRing

void lightLEDs(unsigned char mask)
{
	send(mask);
	P2OUT |= LATCH;
	P2OUT &= ~LATCH;
}

void send(unsigned char s)
{
	unsigned char bit = 0x80;

	while (bit)
	{
		if (s & bit)
		{
			P1OUT |= SI;
		}
		else
		{
			P1OUT &= ~SI;
		}
		bit >>= 1;
		P1OUT |= SCK;
		P1OUT &= ~SCK;
	}
}

// send a mask based on the LEDs that still have high time remaining
// run this method at periodic intervals (base of pwm)
void pwm()
{
	char mask = 0x00;

	int i;
	for (i = 0; i < 8; i++)
	{
		if (ledRing[i])
		{
			mask |= (0x01 << i);
			ledRing[i]--;
		}
	}

	lightLEDs(mask);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// initialize timer
	TimerDefinition timer;
	initializeTimer(&timer);

	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	_BIS_SR(GIE);

	while (1)
	{
		updateTimer(&timer);

		if (timer.milliseconds > 15)
		{
			pwm();
		}
	}

	return 0;
}
