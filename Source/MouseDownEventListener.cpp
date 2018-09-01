#include "MouseDownEventListener.h"

MouseDownEventListener::MouseDownEventListener(UIElement* sender, callback_function callbackFunc) : EventListener(sender, callbackFunc)
{
	std::thread running_thread(&MouseDownEventListener::StartListener, this);
	running_thread.detach();
}

MouseDownEventListener::~MouseDownEventListener()
{
}

void MouseDownEventListener::StartListener()
{
	bool mouseDownOnElement = false;
	bool mouseAlreadDown = false;
	while (!this->listenerShouldStop)
	{
		Sleep(10);
		if (sender == nullptr || sender->GetSourceWindow() == nullptr) continue;

		if (sender->IsVisible() && sender->IsEnabled() && sender->GetSourceWindow()->IsActive())
		{
			if (GetAsyncKeyState(VK_LBUTTON) == 0)
			{
				mouseDownOnElement = false;
				mouseAlreadDown = false;
			}

			// Get Mouse Position
			POINT pos;
			pos.x = Mouse::GetMousePosX(sender->GetSourceWindow());
			pos.y = Mouse::GetMousePosY(sender->GetSourceWindow());

			// If Mouse is Pressed
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !mouseDownOnElement && !mouseAlreadDown)
			{

				if (pos.x < sender->GetPosX() + sender->GetWidth() && pos.x > sender->GetPosX() &&
					pos.y < sender->GetPosY() + sender->GetHeight() && pos.y > sender->GetPosY())
				{
					mouseDownOnElement = true;
				}
			}

			if (mouseDownOnElement)
			{
				CallbackFunc(sender);
			}

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				mouseAlreadDown = true;
			}
		}
	}
}
