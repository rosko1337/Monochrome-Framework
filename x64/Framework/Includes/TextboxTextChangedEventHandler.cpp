#include "TextboxTextChangedEventHandler.h"

TextboxTextChangedEventHandler::TextboxTextChangedEventHandler(UIElement* sender, uiElement_global_callback_function func)
{
	this->sender = sender;
	this->CallbackFunc = func;
	std::thread running_thread(&TextboxTextChangedEventHandler::StartListener, this);
	running_thread.detach();
}

TextboxTextChangedEventHandler::~TextboxTextChangedEventHandler()
{
}

void TextboxTextChangedEventHandler::StartListener()
{
	UITextbox* txtbox = static_cast<UITextbox*>(sender);
	std::string oldText = txtbox->GetText();
	while (true)
	{
		Sleep(10);
		if (sender->GetVisible() && sender->GetEnabled())
		{
			std::string newText = txtbox->GetText();

			if (!newText._Equal(oldText))
			{
				CallbackFunc(sender);
			}

			oldText = newText;
		}
	}
}
