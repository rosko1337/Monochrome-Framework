#include "Mouse.h"

LPWSTR Mouse::CurrentCursorStyle = IDC_ARROW;

float Mouse::GetMousePosX()
{
	POINT pos;
	GetCursorPos(&pos);
	return (float)pos.x;
}

float Mouse::GetMousePosY()
{
	POINT pos;
	GetCursorPos(&pos);
	return (float)pos.y;
}


float Mouse::GetMousePosX(UIWindow* clientWindow)
{
	POINT pos;
	GetCursorPos(&pos);
	if (clientWindow != nullptr)
	{
		ScreenToClient(clientWindow->GetHWND(), &pos);
		pos.x = (pos.x / clientWindow->GetDpiX()) * 96.0f; //  adjusting for the dpi-aware programs
	}
	return (float)pos.x;
}

float Mouse::GetMousePosY(UIWindow* clientWindow)
{
	POINT pos;
	GetCursorPos(&pos);
	if (clientWindow != nullptr)
	{
		ScreenToClient(clientWindow->GetHWND(), &pos);
		pos.y = (pos.y / clientWindow->GetDpiY()) * 96.0f; //  adjusting for the dpi-aware programs
	}
	return (float)pos.y;
}

void Mouse::SetNewCursor(LPWSTR style)
{
	Mouse::CurrentCursorStyle = style;
}

//********* CURSOR STYLES ************//

LPWSTR Mouse::Help = IDC_HELP;
LPWSTR Mouse::Default = IDC_ARROW;
LPWSTR Mouse::Hand = IDC_HAND;
LPWSTR Mouse::Waiting = IDC_WAIT;
LPWSTR Mouse::Text = IDC_IBEAM;
