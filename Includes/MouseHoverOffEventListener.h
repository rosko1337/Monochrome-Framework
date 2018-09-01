#pragma once

#include "EventListener.h"

class MouseHoverOffEventListener : public EventListener
{
public:
	MouseHoverOffEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseHoverOffEventListener();
};

