#pragma once

#include "EventListener.h"

class MouseClickAwayEventListener : public EventListener
{
public:
	MouseClickAwayEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseClickAwayEventListener();
};

