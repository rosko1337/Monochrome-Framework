#include "FTWindow.h"

// Global window customization bitmask
FTBitmask windowSpecs;

FTWindow* FTCreateWindow(int width, int height, LPCWSTR title, LPCWSTR wndexClassName, LRESULT windowProc)
{
	int ws_ex_style = WS_EX_APPWINDOW;
	int ws_style = 0;

	// setting window style params based on user specifications
	if (IsFlagEnabled(&windowSpecs, FT_CLOSE_BUTTON))
	{
		ws_style |= WS_SYSMENU;
		ws_style |= WS_SIZEBOX;
	}
	if (IsFlagEnabled(&windowSpecs, FT_MINIMIZABLE))
	{
		ws_style |= WS_MINIMIZEBOX;
	}
	if (IsFlagEnabled(&windowSpecs, FT_MAXIMIZABLE))
	{
		ws_style |= WS_MAXIMIZEBOX;
	}
	if (IsFlagEnabled(&windowSpecs, FT_HIDE_WINDOW_ICON))
	{
		ws_ex_style |= WS_EX_DLGMODALFRAME;
	}
	if (IsFlagEnabled(&windowSpecs, FT_BORDERLESS))
	{
		ws_style |= WS_POPUP;
	}

	WNDCLASSEX window;
	ZeroMemory(&window, sizeof(WNDCLASSEX));
	window.cbSize = sizeof(WNDCLASSEX);
	HBRUSH brush = (HBRUSH)COLOR_WINDOW;
	window.hbrBackground = brush;
	window.lpfnWndProc = (WNDPROC)windowProc;

	window.lpszClassName = wndexClassName;
	window.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&window);

	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, ws_style, 0, ws_ex_style);

	HWND hWnd = CreateWindowEx(ws_ex_style, wndexClassName, title, ws_style, 100, 100,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, 0);

	if (!hWnd) { return 0; }

	if (IsFlagEnabled(&windowSpecs, FT_NON_RESIZABLE))
	{
		// Remove the ability to resize the window
		// get the styles and properties of your window
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZE | WS_SIZEBOX);
		// set the window with style without titlebar and sizebox
		SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle);
	}

	FTWindow* ftWindow = (FTWindow*)malloc(sizeof(FTWindow));
	ZeroMemory(ftWindow, sizeof(FTWindow));
	ftWindow->hWnd = hWnd;
	ftWindow->width = width;
	ftWindow->height = height;

	return ftWindow;
}

void FTDestroyWindow(FTWindow* window)
{
	DestroyWindow(window->hWnd);
	free(window);
}

void FTShowWindow(FTWindow* window)
{
	ShowWindow(window->hWnd, SW_SHOW);
}

void FTHideWindow(FTWindow* window)
{
	ShowWindow(window->hWnd, SW_HIDE);
}

void FTRestoreWindow(FTWindow* window)
{
	ShowWindow(window->hWnd, SW_RESTORE);
}

void FTFocusWindow(FTWindow* window)
{
	SetFocus(window->hWnd);
}

void FTSetWindowFlags(int flags)
{
	windowSpecs.flags = flags;
}
