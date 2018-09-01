#pragma once

#include "EventListener.h"

class MouseRightClickEventListener : public EventListener
{
public:
	MouseRightClickEventListener(UIElement* sender, callback_function callbackFunc);

	void StartListener();

	~MouseRightClickEventListener();
};

