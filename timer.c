#include "timer.h"

unsigned int g1mSTimeout = 0;

void initializeTimer(TimerDefinition *timer)
{
	timer->milliseconds = 0;
	timer->seconds = 0;
	timer->minutes = 0;
	timer->hours = 0;

	DCOCTL = CALDCO_16MHZ;  		// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_16MHZ;  		// |                        |

	// setup Timer A
	// SMCLK clock, divided by 8, in up mode, and start by clearing the timer
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

	// setup value for comparison
	// 16 MHz / 8 = 2 MHz --> 2000 ticks per ms
	TACCR0 = 2000;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;
}

void updateTimer(TimerDefinition *timer)
{
	while (g1mSTimeout) // if non-zero (software timers are out of sync)
	{
		// While 1 g1mSTimer is out of sync, increment it and decrement timeout
		// Update and run 500 ms and 10 second timers based on the 1 ms global counter
		timer->milliseconds += 1;
		g1mSTimeout -= 1;
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
	g1mSTimeout++;
}
