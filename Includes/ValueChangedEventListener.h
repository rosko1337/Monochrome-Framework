#pragma once

#include "EventListener.h"

class ValueChangedEventListener : public EventListener
{
public:
	ValueChangedEventListener(IUIProgressElement* progressElementSender, value_changed_callback_function valueChangedCallbackFunc);

	void StartListener();

	~ValueChangedEventListener();
};

