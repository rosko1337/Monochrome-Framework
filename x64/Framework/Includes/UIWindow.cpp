#include "UIWindow.h"
#include "UIElement.h"
#include "UIButton.h"
#include "UILabel.h"
#include "Mouse.h"
#include <iostream>

int windowNum = 0;
int currentWindowIndex = 0;
std::vector<Graphics*> graphicsReferences;
std::vector<UIWindow*> windows;

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK childWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

POINT lastMousePos;
POINT currentMousePos;

void CloseWindowOperation(UIElement* sender)
{
	sender->GetSourceWindow()->NotifyCloseOperation();
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

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) 
	{
		for (UIWindow* window : windows)
		{
			DestroyWindow(window->GetHWND());
		}
		PostQuitMessage(0); 
		return 0; 
	}
	if (uMsg == WM_SIZE)
	{
		for (UIWindow* window : windows)
		{
			RECT rect;
			GetWindowRect(window->GetHWND(), &rect);
			if (window->IsBorderless())
			{
				window->GetGraphics()->ResizeRenderTarget(rect.right - rect.left, rect.bottom - rect.top);
			}
			else
			{
				window->GetGraphics()->ResizeRenderTarget(rect.right - rect.left - 22, rect.bottom - rect.top - 43);
			}
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK childWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { return 0; }
	if (uMsg == WM_SIZE)
	{
		for (UIWindow* window : windows)
		{
			RECT rect;
			GetWindowRect(window->GetHWND(), &rect);
			if (window->IsBorderless())
			{
				window->GetGraphics()->ResizeRenderTarget(rect.right - rect.left, rect.bottom - rect.top);
			}
			else
			{
				window->GetGraphics()->ResizeRenderTarget(rect.right - rect.left - 22, rect.bottom - rect.top - 43);
			}
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

UIWindow::UIWindow()
{
	this->graphics = new Graphics();
	graphicsReferences.push_back(graphics);
}

UIWindow::~UIWindow()
{
	delete graphics;
}

void UIWindow::mcCreateWindow(const int width, const int height, const char* windowName)
{
	this->width = width;
	this->height = height;

	std::wstring windowID = L"Window";
	windowID.append(std::to_wstring(windowNum));
	this->windowID = windowNum;
	windowNum++;

	// Setting window flags and style
	FTSetWindowFlags(FT_CLASSIC);
	if (this->borderless)
	{
		FTSetWindowFlags(FT_BORDERLESS);
	} 
	if (!this->resizable)
	{
		FTSetWindowFlags(FT_NON_RESIZABLE);
	}

	// Specifying window proc function and creating the window
	if (windowNum > 1)
	{
		std::string windowNameAsString(windowName);
		this->ftWindow = FTCreateWindow(width, height, std::wstring(windowNameAsString.begin(), windowNameAsString.end()).c_str(), windowID.c_str(), (LRESULT CALLBACK)childWindowProc);
	}
	else
	{
		std::string windowNameAsString(windowName);
		this->ftWindow = FTCreateWindow(width, height, std::wstring(windowNameAsString.begin(), windowNameAsString.end()).c_str(), windowID.c_str(), (LRESULT CALLBACK)windowProc);
	}

	// Initializing Direct2D graphics
	if (!graphics->Init(this->ftWindow->hWnd))
	{
		delete graphics;
		return;
	}

	RECT rect;
	GetWindowRect(this->GetHWND(), &rect);
	this->GetGraphics()->ResizeRenderTarget(rect.right - rect.left, rect.bottom - rect.top);
	windows.push_back(this);
}

void UIWindow::Show()
{
	currentWindowIndex = this->windowID;
	FTShowWindow(ftWindow);
}

void UIWindow::Minimize()
{
	ShowWindow(ftWindow->hWnd, SW_MINIMIZE);
}

void UIWindow::Maximize()
{
	ShowWindow(ftWindow->hWnd, SW_MAXIMIZE);
	RECT mainWindowRect;
	int windowWidth, windowHeight;
	GetWindowRect(ftWindow->hWnd, &mainWindowRect);
	this->height = mainWindowRect.bottom - mainWindowRect.top;
	this->width = mainWindowRect.right - mainWindowRect.left;
}

void UIWindow::Restore()
{
	FTRestoreWindow(ftWindow);
	RECT mainWindowRect;
	int windowWidth, windowHeight;
	GetWindowRect(ftWindow->hWnd, &mainWindowRect);
	this->height = mainWindowRect.bottom - mainWindowRect.top;
	this->width = mainWindowRect.right - mainWindowRect.left;
}

void UIWindow::Hide()
{
	FTHideWindow(ftWindow);
}

void UIWindow::Dispose()
{
	FTDestroyWindow(ftWindow);
}

void UIWindow::Update()
{
	if (shouldCloseOperation)
	{
		this->Dispose();
	}

	// update cursor style
	SetCursor(LoadCursor(NULL, Mouse::CurrentCursorStyle));

	graphics->BeginDraw();
	graphics->ClearScreen(r, g, b);

	if (this->ftWindow->hWnd == GetFocus())
	{
		// set current window index to current window
		currentWindowIndex = this->windowID;
	}

	// menu item callbacks
	for (const auto& pair : menuItemCallbacks)
	{
		if (msg.message == WM_COMMAND)
		{
			if (msg.wParam == pair.first)
			{
				pair.second(this, pair.first);
			}
		}
	}

	// UI Elements
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements.at(i)->GetVisible())
		{
			elements.at(i)->Draw();
		}
	}

	graphics->EndDraw();
	DispatchMessage(&msg);
}

void UIWindow::Add(UIElement* element)
{
	this->elements.push_back(element);
}

void UIWindow::Remove(UIElement* element)
{
	for (int i = 0; i < this->elements.size(); i++)
	{
		if (this->elements.at(i) == element)
		{
			this->elements.erase(this->elements.begin() + (i));
		}
	}
}

bool UIWindow::IsOpened()
{
	return GetMessage(&msg, 0, 0, 0);
}

void UIWindow::MovePosition(int x, int y)
{
	RECT mainWindowRect;
	int windowWidth, windowHeight;
	GetWindowRect(ftWindow->hWnd, &mainWindowRect);
	windowHeight = mainWindowRect.bottom - mainWindowRect.top;
	windowWidth = mainWindowRect.right - mainWindowRect.left;

	MoveWindow(ftWindow->hWnd, x, y, mainWindowRect.right - mainWindowRect.left, mainWindowRect.bottom - mainWindowRect.top, TRUE);
}

void UIWindow::SetMenuBar(UIMenuBar* menuBar)
{
	HMENU windowMenuToolBar = CreateMenu();
	for (int i = 0; i < menuBar->GetMenuList().size(); i++)
	{
		AppendMenuA(windowMenuToolBar, MF_POPUP, (UINT_PTR)menuBar->GetMenuList().at(i)->GetHMenu(), menuBar->GetMenuList().at(i)->GetName().c_str());
	}
	SetMenu(ftWindow->hWnd, windowMenuToolBar);
}

void UIWindow::AddMenuItemCallback(int menuItemID, menu_item_callback_function newCallback)
{
	if (menuItemCallbacks.find(menuItemID) != menuItemCallbacks.end())
	{
		// if a key exists already
		menu_item_callback_function& callbackFunction = menuItemCallbacks.at(menuItemID);
		callbackFunction = newCallback;
	}
	else
	{
		// create a new key and insert it into the map
		std::pair<int, menu_item_callback_function> newPair = std::pair<int, menu_item_callback_function>(menuItemID, newCallback);
		menuItemCallbacks.insert(newPair);
	}
}

void UIWindow::SetWindowIcon(const char* filepath)
{
	HANDLE icon = LoadImageA(NULL, filepath, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	SendMessage(ftWindow->hWnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);
}

void UIWindow::mcCreateModernWindow(int width, int height, const char* windowName, UIButton*& CloseButtonRef, UILabel*& WindowTitleLblRef)
{
	this->SetBorderless(true);
	this->SetBackgroundColor(Color(7, 54, 66, 255));
	this->mcCreateWindow(width, height, windowName);

	// Close Button
	ModernCloseWindowButton = new UIButton(this, "X", "Arial", 14);
	ModernCloseWindowButton->SetTextColor(Color::LightGray);
	ModernCloseWindowButton->SetPosition(this->GetWidth() - 38, 0);
	ModernCloseWindowButton->SetStroke(0);
	ModernCloseWindowButton->SetWidth(38);
	ModernCloseWindowButton->SetHeight(28);
	ModernCloseWindowButton->SetColor(Color(7, 54, 66, 255));
	ModernCloseWindowButton->SetBorderColor(Color(0, 0, 0, 0)); // transparent (invisible border)
	ModernCloseWindowButton->SetHoverOnColor(Color(101, 123, 131, 255));
	ModernCloseWindowButton->SetRoundedCornersRadii(0, 0);
	ModernCloseWindowButton->AddMouseClickEventHandler(CloseWindowOperation);
	this->Add(ModernCloseWindowButton);
	CloseButtonRef = ModernCloseWindowButton;

	ModernWindowDragPanel = new UILabel(this, "", "Verdana", 1, 0, 0, this->GetWidth() - 30, 32, Color(0, 0, 0, 0), 0);
	ModernWindowDragPanel->AddWhileMouseDownEventHandler(WindowDragPanel_WhileMouseDown);
	ModernWindowDragPanel->AddMouseClickEventHandler(WindowDragPanel_OnClick);
	this->Add(ModernWindowDragPanel);

	UILabel* WindowTitle = new UILabel(this, windowName, "Verdana", 12, 20, 0, strlen(windowName) * 14, 30, Color::White, 0);
	WindowTitle->SetTextColor(Color::White);
	WindowTitle->SetTextAllignment(TEXT_ALLIGNMENT_LEFT);
	this->Add(WindowTitle);
	WindowTitleLblRef = WindowTitle;
}
