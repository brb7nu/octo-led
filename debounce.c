#include "debounce.h"

// Returns whether the instantaneous value of signal is low or high
// Called by debounce()
SignalValue getInstantaneousValue(SignalDefinition *signal)
{
	// use port and pin from *signal
	if (*(signal->signalPort) & signal->signalPin)
	{
		// Switch is high
		signal->currentSignalValue = high;
		return high;
	}
	else
	{
		// Switch is low
		signal->currentSignalValue = low;
		return low;
	}
}

// Call this as often as possible to determine the true (debounced) value of a signal
SignalValue debounce(SignalDefinition *signal, TimerDefinition *timer)
{
	//Code must be added to access the internal variable through the SignalDefinition pointer
	SignalValue s = getInstantaneousValue(signal);

	switch (signal->controlState)
	{
		case dbExpectHigh:
			// if the actual reading is different than what was expected
			if (signal->currentSignalValue == low)
			{
				// record time using g1mSTimer
				signal->eventTime = timer->milliseconds;
				signal->controlState = dbValidateLow;
			}

			break;
		case dbValidateHigh:
			if (signal->currentSignalValue == high)
			{				
				// use g1mSTimer to check if time elapsed > threshold
				if (timer->milliseconds - signal->eventTime > signal->validateHighTime)
				{
					signal->validatedSignalValue = high;
					signal->controlState = dbExpectHigh;
				}
			}
			else
			{
				signal->controlState = dbExpectLow;
			}
			break;
		case dbExpectLow:
			// if the actual reading is different than what was expected
			if (signal->currentSignalValue == high)
			{
				// record time using g1mSTimer
				signal->eventTime = timer->milliseconds;
				signal->controlState = dbValidateHigh;
			}

			break;
		case dbValidateLow:
			if (signal->currentSignalValue == low) // button is pressed
			{				
				// use g1mSTimer to check if time elapsed > threshold
				if (timer->milliseconds - signal->eventTime > signal->validateLowTime)
				{
					signal->validatedSignalValue = low;
					signal->controlState = dbExpectLow;
				}
			}
			else
			{
				signal->controlState = dbExpectHigh;
			}
			break;
		default:
			// error, so just go to default state
			signal->controlState = dbExpectHigh;
	}
	//The internal state should be updated here. It should also be returned as a debugging aid.
	return signal->validatedSignalValue;
}
