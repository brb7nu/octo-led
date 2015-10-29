#include "button.h"

void updateButtonState(ButtonDefinition *button, TimerDefinition *timer)
{
	ButtonState startingState = button->state; // starting state

	debounce(&(button->buttonSignal), timer);

	switch (startingState)
	{
		case pressed:
			if (button->buttonSignal.validatedSignalValue == high)
			{
				button->state = unpressed;
			}
			break;
		case unpressed:
			if (button->buttonSignal.validatedSignalValue == low)
			{
				button->state = pressed;
				button->count++;
			}
			break;
		default:
			break;

	}
}
