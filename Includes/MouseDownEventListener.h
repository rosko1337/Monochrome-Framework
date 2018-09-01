#pragma once

#include "EventListener.h"

class MouseDownEventListener : public EventListener
{
public:
	MouseDownEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseDownEventListener();
};

