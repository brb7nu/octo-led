#include "timer.h"

unsigned int g10uSTimeout = 0;

void initializeTimer(TimerDefinition *timer)
{
	timer->microseconds = 0;
	timer->milliseconds = 0;
	timer->seconds = 0;
	timer->minutes = 0;
	timer->hours = 0;

	DCOCTL = CALDCO_16MHZ;  		// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_16MHZ;  		// |                        |

	// setup Timer A
	// SMCLK clock, divided by 4, in up mode, and start by clearing the timer
	TACTL = TASSEL_2 | ID_2 | MC_1 | TACLR;

	// setup value for comparison
	// 16 MHz / 4 = 4 MHz --> 40 ticks per 10 microseconds
	TACCR0 = 40;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;
}

void updateTimer(TimerDefinition *timer)
{
	while (g10uSTimeout) // if non-zero (software timers are out of sync)
	{
		// While 1 g10uSTimer is out of sync, increment it and decrement timeout
		// Update and run 500 ms and 10 second timers based on the 1 ms global counter
		timer->microseconds += 10;
		g10uSTimeout--;
	}
	while (timer->microseconds >= 1000)
	{
		timer->milliseconds += 1;
		timer->microseconds -= 1000;
	}
	while (timer->milliseconds >= 1000)
	{
		timer->seconds += 1;
		timer->milliseconds -= 1000;
	}
	while (timer->seconds >= 60)
	{
		timer->minutes += 1;
		timer->seconds -= 60;
	}
	while (timer->minutes >= 60)
	{
		timer->hours += 1;
		timer->minutes -= 60;
	}
}

#pragma vector = TIMER0_A0_VECTOR // Timer A interrupt service routine
__interrupt void TimerA0_routine(void)
{
	g10uSTimeout++;
}
