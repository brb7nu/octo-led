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

// stores the number of milliseconds each led must stay on
int ledRing[8];

// NO need for LED mask because it just holds the non-zero values in ledRing

void send(char mask)
{
	// SPI send the mask to the LED ring
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

	send(mask);
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// initialize timer
	TimerDefinition timer;
	initializeTimer(&timer);

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
