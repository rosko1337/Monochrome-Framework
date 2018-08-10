#include "GlobalHoverOffEventHandler.h"

GlobalHoverOffEventHandler::GlobalHoverOffEventHandler(UIElement* sender, uiElement_global_callback_function func)
{
	this->sender = sender;
	this->CallbackFunc = func;
	std::thread running_thread(&GlobalHoverOffEventHandler::StartListener, this);
	running_thread.detach();
}

GlobalHoverOffEventHandler::~GlobalHoverOffEventHandler()
{
}

void GlobalHoverOffEventHandler::StartListener()
{
	bool mouseOverElement = false;
	while (true)
	{
		Sleep(10);
		if (sender->GetVisible() && sender->GetEnabled() && sender->GetSourceWindow()->IsActive())
		{
			float mouseX = Mouse::GetMousePosX(sender->GetSourceWindow());
			float mouseY = Mouse::GetMousePosY(sender->GetSourceWindow());

			bool isMouseOverElementCurrently =
				(mouseX < sender->GetPosX() + sender->GetWidth() && mouseX > sender->GetPosX() &&
					mouseY < sender->GetPosY() + sender->GetHeight() && mouseY > sender->GetPosY());

			if (isMouseOverElementCurrently)
			{
				mouseOverElement = true;
			}

			if (mouseOverElement && !isMouseOverElementCurrently)
			{
				CallbackFunc(sender);
				mouseOverElement = false;
			}
		}
	}
}
