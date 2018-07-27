#include "MCWindowManager.h"

POINT lastMousePos;
POINT currentMousePos;

UIButton* CloseWindowButton;
UIButton* MaximizeWindowButton;
UIButton* MinimizeWindowButton;
UILabel* WindowDragPanel;

bool WindowMaximized = false;

void CloseWindowOperation(UIElement* sender)
{
	sender->GetSourceWindow()->NotifyCloseOperation();
}

void MaximizeWindowOperation(UIElement* sender)
{
	if (!WindowMaximized)
	{
		sender->GetSourceWindow()->MovePosition(0, 0);
		sender->GetSourceWindow()->Maximize();
		CloseWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 32, 0);
		MaximizeWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 65, 0);
		MinimizeWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 97, 0);
		WindowDragPanel->SetWidth(sender->GetSourceWindow()->GetWidth() - 120);
		WindowMaximized = true;
	}
	else
	{
		sender->GetSourceWindow()->MovePosition(100, 100);
		sender->GetSourceWindow()->Restore();
		CloseWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 34, 0);
		MaximizeWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 67, 0);
		MinimizeWindowButton->SetPosition(sender->GetSourceWindow()->GetWidth() - 100, 0);
		WindowDragPanel->SetWidth(sender->GetSourceWindow()->GetWidth() - 120);
		WindowMaximized = false;
	}
}

void MinimizeWindowOperation(UIElement* sender)
{
	sender->GetSourceWindow()->Minimize();
}

void WindowDragPanel_OnClick(UIElement* sender)
{
	GetCursorPos(&lastMousePos);
	ScreenToClient(sender->GetSourceWindow()->GetHWND(), &lastMousePos);
}

void WindowDragPanel_WhileMouseDown(UIElement* sender)
{
	GetCursorPos(&currentMousePos);
	sender->GetSourceWindow()->MovePosition(currentMousePos.x - lastMousePos.x, currentMousePos.y - lastMousePos.y);
}

UIWindow* mcCreateModernWindow(int width, int height, const char* windowName)
{
	UIWindow* window = new UIWindow();
	window->SetResizable(false);
	window->SetBorderless(true);
	window->SetBackgroundColor(Color::Navy);
	window->mcCreateWindow(width, height, windowName);

	// Close Button
	CloseWindowButton = new UIButton(window, "X", "Palatino", 14);
	CloseWindowButton->SetPosition(window->GetWidth() - 14, 0);
	CloseWindowButton->SetStroke(0);
	CloseWindowButton->SetWidth(34);
	CloseWindowButton->SetHeight(32);
	CloseWindowButton->SetColor(new Color(80, 80, 80, 255));
	CloseWindowButton->SetBorderColor(new Color(70, 70, 70, 0)); // transparent (invisible border)
	CloseWindowButton->SetHoverOnColor(Color::WineRed);
	CloseWindowButton->SetRoundedCornersRadii(1, 1);
	CloseWindowButton->AddMouseClickEventHandler(CloseWindowOperation);
	window->Add(CloseWindowButton);

	// Maximize Button
	MaximizeWindowButton = new UIButton(window, "☐", "Palatino", 14);
	MaximizeWindowButton->SetPosition(window->GetWidth() - 47, 0);
	MaximizeWindowButton->SetStroke(0);
	MaximizeWindowButton->SetWidth(34);
	MaximizeWindowButton->SetHeight(32);
	MaximizeWindowButton->SetColor(new Color(80, 80, 80, 255));
	MaximizeWindowButton->SetBorderColor(new Color(70, 70, 70, 0)); // transparent (invisible border)
	MaximizeWindowButton->SetHoverOnColor(Color::DarkGray);
	MaximizeWindowButton->SetRoundedCornersRadii(1, 1);
	MaximizeWindowButton->AddMouseClickEventHandler(MaximizeWindowOperation);
	window->Add(MaximizeWindowButton);

	// Maximize Minimize
	MinimizeWindowButton = new UIButton(window, "__", "Palatino", 14);
	MinimizeWindowButton->SetPosition(window->GetWidth() - 80, 0);
	MinimizeWindowButton->SetStroke(0);
	MinimizeWindowButton->SetWidth(34);
	MinimizeWindowButton->SetHeight(32);
	MinimizeWindowButton->SetColor(new Color(80, 80, 80, 255));
	MinimizeWindowButton->SetBorderColor(new Color(70, 70, 70, 0)); // transparent (invisible border)
	MinimizeWindowButton->SetHoverOnColor(Color::DarkGray);
	MinimizeWindowButton->SetRoundedCornersRadii(1, 1);
	MinimizeWindowButton->AddMouseClickEventHandler(MinimizeWindowOperation);
	window->Add(MinimizeWindowButton);

	WindowDragPanel = new UILabel(window, "", "Verdana", 1, 0, 0, window->GetWidth() - 30, 32, new Color(0, 0, 0, 0), 0);
	WindowDragPanel->AddWhileMouseDownEventHandler(WindowDragPanel_WhileMouseDown);
	WindowDragPanel->AddMouseClickEventHandler(WindowDragPanel_OnClick);
	window->Add(WindowDragPanel);

	UILabel* WindowTitle = new UILabel(window, windowName, "Verdana", 14, 24, 0, strlen(windowName) * 14, 32, Color::White, 0);
	WindowTitle->SetTextColor(Color::White);
	WindowTitle->SetTextAllignment(TEXT_ALLIGNMENT_LEFT);
	window->Add(WindowTitle);

	return window;
}