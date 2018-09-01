#include "ValueChangedEventListener.h"

ValueChangedEventListener::ValueChangedEventListener(IUIProgressElement* progressElementSender, value_changed_callback_function valueChangedCallbackFunc)
	: EventListener(progressElementSender, valueChangedCallbackFunc)
{
	std::thread running_thread(&ValueChangedEventListener::StartListener, this);
	running_thread.detach();
}

ValueChangedEventListener::~ValueChangedEventListener()
{
}

void ValueChangedEventListener::StartListener()
{
	float previousValue = progressElementSender->GetValue();
	while (!this->listenerShouldStop)
	{
		Sleep(2);
		if (progressElementSender == nullptr) continue;

		float currentValue = progressElementSender->GetValue();

		if (previousValue != currentValue)
		{
			ValueChangedCallbackFunc(progressElementSender);
		}
		
		previousValue = currentValue;
	}
}
