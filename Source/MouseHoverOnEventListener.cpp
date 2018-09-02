#include "MouseHoverOnEventListener.h"

MouseHoverOnEventListener::MouseHoverOnEventListener(UIElement* sender, callback_function callbackFunc) : EventListener(sender, callbackFunc)
{
	std::thread running_thread(&MouseHoverOnEventListener::StartListener, this);
	running_thread.detach();
}

void MouseHoverOnEventListener::StartListener()
{
	bool mouseOverElement = false;
	while (!this->listenerShouldStop)
	{
		Sleep(2);
		if (sender == nullptr || sender->GetSourceWindow() == nullptr) continue;

		if (sender->IsVisible() && sender->IsEnabled() && sender->GetSourceWindow()->IsActive())
		{
			float mouseX = Mouse::GetMousePosX(sender->GetSourceWindow());
			float mouseY = Mouse::GetMousePosY(sender->GetSourceWindow());

			if (mouseX < sender->GetPosX() + sender->GetWidth() && mouseX > sender->GetPosX() &&
				mouseY < sender->GetPosY() + sender->GetHeight() && mouseY > sender->GetPosY())
			{
				if (!mouseOverElement)
				{
					CallbackFunc(sender);
					mouseOverElement = true;
				}
			}
			else
			{
				mouseOverElement = false;
			}
		}
	}
}

MouseHoverOnEventListener::~MouseHoverOnEventListener()
{
}