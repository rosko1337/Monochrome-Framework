#include "GlobalHoverOnEventHandler.h"

GlobalHoverOnEventHandler::GlobalHoverOnEventHandler(UIElement* sender, uiElement_global_callback_function func)
{
	this->sender = sender;
	this->CallbackFunc = func;
	std::thread running_thread(&GlobalHoverOnEventHandler::StartListener, this);
	running_thread.detach();
}

GlobalHoverOnEventHandler::~GlobalHoverOnEventHandler()
{
}

void GlobalHoverOnEventHandler::StartListener()
{
	bool mouseOverElement = false;
	while (true)
	{
		Sleep(10);
		if (sender->GetVisible() && sender->GetEnabled() && sender->GetSourceWindow()->IsActive())
		{
			float mouseX = Mouse::GetMousePosX(sender->GetSourceWindow());
			float mouseY = Mouse::GetMousePosY(sender->GetSourceWindow());


			if (mouseX < sender->GetPosX() + sender->GetWidth() && mouseX > sender->GetPosX() &&
				mouseY < sender->GetPosY() + sender->GetHeight() && mouseY > sender->GetPosY())
			{
				CallbackFunc(sender);
				mouseOverElement = true;
			}
			else
			{
				mouseOverElement = false;
			}
		}
	}
}
