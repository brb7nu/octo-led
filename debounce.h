#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <msp430.h>
#include "timer.h"

typedef enum {low, high} SignalValue;

typedef enum {dbExpectHigh, dbValidateHigh, dbExpectLow, dbValidateLow} DebounceState;

typedef struct
{
	DebounceState controlState;
	SignalValue currentSignalValue; // {instantaneous, from getInstantaneousValue}
	SignalValue validatedSignalValue;    // {debounced value of the signal from state machine}
	char* signalPort;
	char signalPin;
	unsigned int validateHighTime;
	unsigned int validateLowTime;
	unsigned int eventTime;
} SignalDefinition;

SignalValue getInstantaneousValue(SignalDefinition *signal);
SignalValue debounce(SignalDefinition *signal, TimerDefinition *timer);

#endif
