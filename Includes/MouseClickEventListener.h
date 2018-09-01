#pragma once

#include "EventListener.h"

class MouseClickEventListener : public EventListener
{
public:
	MouseClickEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseClickEventListener();
};

