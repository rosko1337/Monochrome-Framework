#include "UIWindow.h"
#include "UIElement.h"
#include "Mouse.h"

int windowNum = 0;
Graphics* graphicsReference;

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }
	if (uMsg == WM_SIZE)
	{
		RECT newSize;
		GetClientRect(hwnd, &newSize);
		graphicsReference->ResizeRenderTarget(newSize.right - newSize.left, newSize.bottom - newSize.top);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK childWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { return 0; }
	if (uMsg == WM_SIZE)
	{
		RECT newSize;
		GetClientRect(hwnd, &newSize);
		graphicsReference->ResizeRenderTarget(newSize.right - newSize.left, newSize.bottom - newSize.top);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

UIWindow::UIWindow()
{
	this->graphics = new Graphics();
	graphicsReference = this->graphics;
}

UIWindow::~UIWindow()
{

}

void UIWindow::mcCreateWindow(const int width, const int height, LPCWSTR windowName)
{
	this->width = width;
	this->height = height;

	std::wstring windowID = L"Window";
	windowID.append(std::to_wstring(windowNum));
	windowNum++;

	WNDCLASSEX window;
	ZeroMemory(&window, sizeof(WNDCLASSEX));
	window.cbSize = sizeof(WNDCLASSEX);
	window.hbrBackground = (HBRUSH)COLOR_WINDOW;
	// Window Proc
	if (windowNum > 1)
	{
		window.lpfnWndProc = childWindowProc;
	}
	else
	{
		window.lpfnWndProc = windowProc;
	}

	window.lpszClassName = windowID.c_str();
	window.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&window);

	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, windowID.c_str(), windowName, WS_OVERLAPPEDWINDOW, startingLocationX, startingLocationY,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, 0);

	if (borderless)
	{
		AdjustWindowRectEx(&rect, WS_POPUP, false, 0);
		hWnd = CreateWindowEx(0, windowID.c_str(), windowName, WS_POPUP, startingLocationX, startingLocationY,
			rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, 0);
		SetWindowLongPtr(hWnd, GWL_STYLE, 0); // remove all window styles
	}

	if (!hWnd) { return; }

	if (!this->resizable)
	{
		// get the styles and properties of your window
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZE | WS_SIZEBOX);
		// set the window with style without titlebar and sizebox
		SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle);
	}

	if (!graphics->Init(hWnd))
	{
		delete graphics;
		return;
	}

	this->hWnd = hWnd;
}

void UIWindow::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void UIWindow::Minimize()
{
	ShowWindow(hWnd, SW_MINIMIZE);
}

void UIWindow::Maximize()
{
	ShowWindow(hWnd, SW_MAXIMIZE);
	RECT mainWindowRect;
	int windowWidth, windowHeight;
	GetWindowRect(hWnd, &mainWindowRect);
	this->height = mainWindowRect.bottom - mainWindowRect.top;
	this->width = mainWindowRect.right - mainWindowRect.left;
}

void UIWindow::Restore()
{
	ShowWindow(hWnd, SW_RESTORE);
	RECT mainWindowRect;
	int windowWidth, windowHeight;
	GetWindowRect(hWnd, &mainWindowRect);
	this->height = mainWindowRect.bottom - mainWindowRect.top;
	this->width = mainWindowRect.right - mainWindowRect.left;
}

void UIWindow::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

void UIWindow::Dispose()
{
	DestroyWindow(hWnd);
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
	GetWindowRect(hWnd, &mainWindowRect);
	windowHeight = mainWindowRect.bottom - mainWindowRect.top;
	windowWidth = mainWindowRect.right - mainWindowRect.left;

	MoveWindow(hWnd, x, y, mainWindowRect.right - mainWindowRect.left, mainWindowRect.bottom - mainWindowRect.top, TRUE);
}
