#ifndef TIMER_H
#define TIMER_H

#include <msp430.h>

extern unsigned int timeout;

typedef struct
{
	unsigned int microseconds;
	unsigned int milliseconds;
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
} TimerDefinition;

void initializeTimer(TimerDefinition *timer);
void updateTimer(TimerDefinition *timer);
__interrupt void TimerA0_routine(void);

#endif
