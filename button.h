#ifndef BUTTON_H
#define BUTTON_H

#include <msp430.h>
#include "debounce.h"

typedef enum {pressed, unpressed} ButtonState;

typedef struct
{
	SignalDefinition buttonSignal;
	ButtonState state;
	signed int count;
} ButtonDefinition;

void updateButtonState(ButtonDefinition *button, TimerDefinition *timer);

#endif
