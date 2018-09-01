#include "MouseHoverOffEventListener.h"

MouseHoverOffEventListener::MouseHoverOffEventListener(UIElement* sender, callback_function callbackFunc) : EventListener(sender, callbackFunc)
{
	std::thread running_thread(&MouseHoverOffEventListener::StartListener, this);
	running_thread.detach();
}


void MouseHoverOffEventListener::StartListener()
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
					mouseOverElement = true;
				}
			}
			else
			{
				if (mouseOverElement)
				{
					CallbackFunc(sender);
					mouseOverElement = false;
				}
			}
		}
	}
}

MouseHoverOffEventListener::~MouseHoverOffEventListener()
{
}
