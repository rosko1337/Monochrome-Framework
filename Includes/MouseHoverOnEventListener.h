#pragma once

#include "EventListener.h"

class MouseHoverOnEventListener : public EventListener
{
public:
	MouseHoverOnEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseHoverOnEventListener();
};

