#include "GlobalWhileMouseDownEventHandler.h"

GlobalWhileMouseDownEventHandler::GlobalWhileMouseDownEventHandler(UIElement* sender, uiElement_global_callback_function func)
{
	this->CallbackFunc = func;
	this->sender = sender;
	std::thread running_thread(&GlobalWhileMouseDownEventHandler::StartListener, this);
	running_thread.detach();
}

GlobalWhileMouseDownEventHandler::~GlobalWhileMouseDownEventHandler()
{
}

void GlobalWhileMouseDownEventHandler::StartListener()
{
	bool mouseDownOnElement = false;
	bool mouseAlreadDown = false;
	while (true)
	{
		Sleep(10);

		if (sender->GetVisible() && sender->GetEnabled())
		{
			if (GetAsyncKeyState(VK_LBUTTON) == 0)
			{
				mouseDownOnElement = false;
				mouseAlreadDown = false;
			}

			// Get Mouse Position
			POINT pos;
			GetCursorPos(&pos);
			ScreenToClient(sender->GetSourceWindow()->GetHWND(), &pos);

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