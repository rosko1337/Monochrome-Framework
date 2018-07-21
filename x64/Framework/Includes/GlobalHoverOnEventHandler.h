#pragma once

#include "EventHandler.h"
#include "Mouse.h"

class GlobalHoverOnEventHandler : public EventHandler
{
public:
	GlobalHoverOnEventHandler(UIElement* sender, uiElement_global_callback_function func);

	void StartListener();

	~GlobalHoverOnEventHandler();

private:
	UIElement* sender;
	uiElement_global_callback_function CallbackFunc;
};

