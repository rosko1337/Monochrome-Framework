#pragma once

#include "UIElement.h"
#include "IUIProgressElement.h"
#include <thread>
#include "Mouse.h"

class EventListener
{
public:
	// Starts loop that listens for an event
	virtual void StartListener() = 0;

	// Completely stops the listening loop
	virtual void StopListener() { this->listenerShouldStop = true; }

protected:
	// Callback function prototype
	typedef void(*callback_function)(UIElement*);
	typedef void(*value_changed_callback_function)(IUIProgressElement*);

	EventListener(UIElement* sender, callback_function callbackFunc);
	EventListener(IUIProgressElement* sender, value_changed_callback_function valueChangedCallbackFunc);

	// Element that send the event
	UIElement* sender;
	IUIProgressElement* progressElementSender;

	// Callback functions
	callback_function CallbackFunc;
	value_changed_callback_function ValueChangedCallbackFunc;

	// Stop signal to break out of listening loop
	bool listenerShouldStop = false;
};

