#include "EventListener.h"

EventListener::EventListener(UIElement * sender, callback_function callbackFunc)
{
	this->CallbackFunc = callbackFunc;
	this->sender = sender;
}

EventListener::EventListener(IUIProgressElement* sender, value_changed_callback_function valueChangedCallbackFunc)
{
	this->ValueChangedCallbackFunc = valueChangedCallbackFunc;
	this->progressElementSender = sender;
}
