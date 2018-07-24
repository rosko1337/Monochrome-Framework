#pragma once

#include "EventHandler.h"
#include "UITextbox.h"

class TextboxTextChangedEventHandler : public EventHandler
{
public:
	TextboxTextChangedEventHandler(UIElement* sender, uiElement_global_callback_function func);

	void StartListener();

	~TextboxTextChangedEventHandler();

private:
	UIElement * sender;
	uiElement_global_callback_function CallbackFunc;
};

