#include "MouseRightClickEventListener.h"

MouseRightClickEventListener::MouseRightClickEventListener(UIElement* sender, callback_function callbackFunc) : EventListener(sender, callbackFunc)
{
	std::thread running_thread(&MouseRightClickEventListener::StartListener, this);
	running_thread.detach();
}

void MouseRightClickEventListener::StartListener()
{
	bool pressed = false;
	while (!this->listenerShouldStop)
	{
		Sleep(2);
		if (sender == nullptr || sender->GetSourceWindow() == nullptr) continue;

		if (sender->IsVisible() && sender->IsEnabled() && sender->GetSourceWindow()->IsActive())
		{
			if (GetAsyncKeyState(VK_RBUTTON) == 0)
				pressed = false;

			Sleep(2);
			if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !pressed)
			{
				pressed = true;

				// Left Click Action
				POINT pos;
				pos.x = Mouse::GetMousePosX(sender->GetSourceWindow());
				pos.y = Mouse::GetMousePosY(sender->GetSourceWindow());

				if (pos.x < sender->GetPosX() + sender->GetWidth() && pos.x > sender->GetPosX() &&
					pos.y < sender->GetPosY() + sender->GetHeight() && pos.y > sender->GetPosY())
				{
					CallbackFunc(sender);
				}
			}
		}
	}
}

MouseRightClickEventListener::~MouseRightClickEventListener()
{
}
