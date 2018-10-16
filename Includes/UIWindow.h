#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "Graphics.h"
#include <Windows.h>
#include <map>
#include <string>
#include "Color.h"
#include "UIMenuBar.h"
#include "FTWindow.h"

class UIMenu;
class UIView;
class UIButton;
class UILabel;
class UIImage;

class UIWindow
{
public:
	UIWindow();

	// Creates classic window with default windows border
	void mcCreateWindow(int width, int height, const char* windowName);

	// Creates window with flat modern design with custom close button and window title label
	void mcCreateModernWindow(int width, int height, const char* windowName);

	// Opens and presents the window
	void Show();

	// Maximizes the window if the window is resizable
	void Maximize();

	// Minimizes the window if the window is resizable
	void Minimize();

	// Opens the window back up after the Minimize() action
	void Restore();

	// Hides the window
	void Hide();

	// Sends a direct message to the window loop telling the window to close
	// but it is not thread safe so it is suggested to use NotifyCloseOperation instead
	void Dispose();

	// Draws all UI Elements and checks for any notify operations
	void Update();

	// Returns whether window is opened or closed
	bool IsOpened();

	// If window was created with mcCreateModernWindow(), it returns the pointer to the close button
	UIButton* GetModernWindowCloseButton() { return this->ModernWindowCloseButton; }

	// If window was created with mcCreateModernWindow(), it returns the pointer to the window title label
	UILabel* GetModernWindowTitleLabel() { return this->ModernWindowTitleLabel; }

	// If window was created with mcCreateModernWindow(), it returns the pointer to the label used as panel for dragging the window
	UILabel* GetModernWindowDragPanel() { return this->ModernWindowDragPanel; }
	
	// Background color componenets
	uint8_t r = 120, g = 120, b = 120;

	// Returns HWND handle to the window
	HWND GetHWND() { return this->ftWindow->hWnd; }

	// Returns Graphics object containing the RenderTarget
	Graphics* GetGraphics() { return this->graphics; }

	// callback function prototype for menu items
	typedef void(*menu_item_callback_function)(UIWindow*, int);
	
	// Sets current view
	void SetView(UIView* view);

	// Tells the window to render static elements of the window on top of everything else
	void ShouldRenderStaticViewLast(bool state) { this->shouldRenderStaticViewLast = state; }

	// Returns the width of the window in pixels
	int GetWidth() { return this->width; }

	// Returns the height of the window in pixels
	int GetHeight() { return this->height; }

	// Returns display X DPI
	float GetDpiX() { return this->dpiX; }

	// Returns display Y DPI
	float GetDpiY() { return this->dpiY; }

	// Returns window ID that was assigned during window creation
	int GetWindowID() { return this->windowID; }

	// Returns whether the window has default windows border or not
	bool IsBorderless() { return this->borderless; }

	// Returns true if window is in focus and is active
	bool IsActive() 
	{
		if (this->ftWindow->hWnd != NULL)
		{
			return this->ftWindow->hWnd == GetForegroundWindow();
		}
		else
		{
			return false;
		}
	}

	// Enables/Disables resize operation of the window
	void SetResizable(bool state) { this->resizable = state; }

	// Sets background color of the window
	void SetBackgroundColor(Color color);

	// Returns background color of the window
	Color GetBackgroundColor() { return Color(this->r, this->g, this->b, 255); }

	// Removes default windows border from the window
	void SetBorderless(bool state) { this->borderless = state; }

	// Moves the window by specified number of pixels
	void MovePosition(int x, int y);

	// Sets background image of the window
	void SetBackgroundImage(UIImage* img);

	// Loads background image file
	void LoadBackgroundImage(const char* imgFilePath);

	// Sends a window message telling the window to close (thread safe)
	void NotifyCloseOperation() { shouldCloseOperation = true; }

	// Sends a window message telling the RenderTarget to resize according to the new window size
	void NotifyResizeOperation() { shouldResize = true; }

	// Returns most recent MSG struct from win32api
	MSG& GetSystemMessage() { return this->msg; }

	// If window has default windows border, it allows to change the window icon
	void SetWindowIcon(const char* filepath);

	// Sets menu bar of the window containing all the menus and menu items
	void SetMenuBar(UIMenuBar* menuBar);

	// Adds callback function to the menu item based on its ID
	void AddMenuItemCallback(int menuItemID, menu_item_callback_function callback);

	~UIWindow();
private:
	void DrawDirect2DUIContents();
	FTWindow* ftWindow;
	Graphics* graphics;
	int windowID = 0;
	bool resizable = true;
	bool borderless = false;
	int width = 0, height = 0;
	float dpiX, dpiY;
	UIView* staticView = 0;
	UIView* currentView = nullptr;
	MSG msg;
	bool shouldRenderStaticViewLast = false;
	bool shouldCloseOperation = false;
	bool windowIsOpened = false;
	bool shouldResize = false;
	std::map<int, menu_item_callback_function> menuItemCallbacks;

	// base UI elements for the modern flat window design
	UIButton* ModernWindowCloseButton = nullptr;
	UILabel* ModernWindowTitleLabel = nullptr;
	UILabel* ModernWindowDragPanel = nullptr;
	UIImage* backgroundImage = nullptr;

	void ResizeRenderTarget();
};

#endif // !UIWINDOW_H