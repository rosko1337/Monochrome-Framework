#pragma once

#include "EventHandler.h"
#include "UIElement.h"
#include <thread>

class GlobalRightMouseClickEventHandler : public EventHandler
{
public:
	GlobalRightMouseClickEventHandler(UIElement* sender, uiElement_global_callback_function func);

	void StartListener();

	~GlobalRightMouseClickEventHandler();

private:
	UIElement * sender;
	uiElement_global_callback_function CallbackFunc;
};

