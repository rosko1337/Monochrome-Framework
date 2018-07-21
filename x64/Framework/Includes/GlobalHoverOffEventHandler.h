#pragma once

#include "EventHandler.h"
#include "Mouse.h"

class GlobalHoverOffEventHandler : public EventHandler
{
public:
	GlobalHoverOffEventHandler(UIElement* sender, uiElement_global_callback_function func);

	void StartListener();

	~GlobalHoverOffEventHandler();

private:
	UIElement* sender;
	uiElement_global_callback_function CallbackFunc;
};

