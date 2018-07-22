#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "Graphics.h"
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Color.h"
#include "UIMenuBar.h"

class UIMenu;

class UIElement;

class UIWindow
{
public:
	UIWindow();

	void mcCreateWindow(const int width, const int height, LPCWSTR windowName);
	void Show();
	void Maximize();
	void Minimize();
	void Restore();
	void Hide();
	void Dispose();
	void Update();
	bool IsOpened();

	uint8_t r = 120, g = 120, b = 120;

	HWND GetHWND() { return this->hWnd; }
	Graphics* GetGraphics() { return this->graphics; }
	typedef void(*menu_item_callback_function)(UIWindow*);

	void Add(UIElement* element);
	void Remove(UIElement* element);

	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	void SetResizable(bool state) { this->resizable = state; }
	void SetBackgroundColor(Color* color) { this->r = color->r; this->g = color->g; this->b = color->b; }

	void SetBorderless(bool state) { this->borderless = state; }
	void SetWindowLocation(int x, int y) { startingLocationX = x; startingLocationY = y; }
	void MovePosition(int x, int y);

	void NotifyCloseOperation() { shouldCloseOperation = true; }
	MSG& GetSystemMessage() { return this->msg; }
	void SetWindowIcon(LPCWSTR filepath);

	void SetMenuBar(UIMenuBar* menuBar);
	void AddMenuItemCallback(int menuItemID, menu_item_callback_function callback);

	~UIWindow();
private:
	HWND hWnd;
	Graphics* graphics;
	bool resizable = true;
	bool borderless = false;
	int width = 0, height = 0;
	int startingLocationX = 100, startingLocationY = 100;
	std::vector<UIElement*> elements;
	MSG msg;
	bool shouldCloseOperation = false;
	std::map<int, menu_item_callback_function> menuItemCallbacks;
};

#endif // !UIWINDOW_H