#include "UIWindow.h"
#include "UIView.h"
#include "Mouse.h"
#include "UIButton.h"
#include "UILabel.h"

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
			window->NotifyCloseOperation();
			DestroyWindow(window->GetHWND());
		}
		return 0;
	}
	if (uMsg == WM_SIZE)
	{
		for (UIWindow* window : windows)
		{
			if (window->GetHWND() == hwnd)
			{
				window->NotifyResizeOperation();
			}
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK childWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_QUIT)
	{

	}

	if (uMsg == WM_DESTROY) 
	{
		for (int i = 0; i < windows.size(); i++)
		{
			if (windows.at(i)->GetHWND() == hwnd)
			{
				windows.at(i)->NotifyCloseOperation();
				DestroyWindow(windows.at(i)->GetHWND());
			}
		}
		return 0; 
	}
	if (uMsg == WM_SIZE)
	{
		for (UIWindow* window : windows)
		{
			if (window->GetHWND() == hwnd)
			{
				window->NotifyResizeOperation();
			}
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

UIWindow::UIWindow()
{
	// Preconfiguring the application to be High-DPI-Aware so that
	// text renders properly on high-dpi monitors
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

	this->graphics = new Graphics();
	graphics->CreateResources();
	graphicsReferences.push_back(graphics);
	staticView = new UIView();
}

UIWindow::~UIWindow()
{
	delete graphics;
}

void UIWindow::mcCreateWindow(int width, int height, const char* windowName)
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

	// Get Dpi of High DPI Monitors
	float dpiX, dpiY;
	graphics->GetMonitorDpi(&dpiX, &dpiY);

	// adjusting width and height of the window to the monitor's DPI
	width = (int)(width * dpiX / 96.0f);
	height = (int)(height * dpiY / 96.0f);

	this->dpiX = dpiX;
	this->dpiY = dpiY;

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
	windowIsOpened = true;

	std::thread d2d1updatingLoop(&UIWindow::DrawDirect2DUIContents, this);
	d2d1updatingLoop.detach();
}

void UIWindow::DrawDirect2DUIContents()
{
	while (this->IsOpened())
	{
		if (this->shouldResize)
		{
			ResizeRenderTarget();
			shouldResize = false;
		}

		graphics->BeginDraw();
		graphics->ClearScreen(r, g, b);

		// Drawing Static UI elements first if set
		if (!shouldRenderStaticViewLast)
		{
			if (staticView != nullptr)
			{
				for (UIElement* staticElement : staticView->GetElements())
				{
					if (staticElement == nullptr) { this->staticView->Remove(staticElement); continue; }
					if (staticElement->IsVisible())
					{
						staticElement->Draw(graphics);
					}
				}
			}
		}

		// Drawing dynamic UI elements from current view
		if (currentView != nullptr)
		{
			for (UIElement* element : currentView->GetElements())
			{
				if (element == nullptr) { this->currentView->Remove(element); continue; }
				if (element->IsVisible())
				{
					element->Draw(graphics);
				}
			}
		}

		// If user selects to do so, static view will be rendered on top of everything else
		if (shouldRenderStaticViewLast)
		{
			if (staticView != nullptr)
			{
				for (UIElement* staticElement : staticView->GetElements())
				{
					if (staticElement == nullptr) { this->staticView->Remove(staticElement); continue; }
					if (staticElement->IsVisible())
					{
						staticElement->Draw(graphics);
					}
				}
			}
		}

		graphics->EndDraw();
	}
}

void UIWindow::ResizeRenderTarget()
{
	RECT rect;
	GetWindowRect(ftWindow->hWnd, &rect);
	if (this->IsBorderless())
	{
		this->GetGraphics()->ResizeRenderTarget(rect.right - rect.left, rect.bottom - rect.top);
	}
	else
	{
		this->GetGraphics()->ResizeRenderTarget(rect.right - rect.left - 22, rect.bottom - rect.top - 43);
	}
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
	int windowWidth = 0, windowHeight = 0;
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
	GetMessage(&msg, 0, 0, 0);

	if (shouldCloseOperation)
	{
		DispatchMessage(&msg);
		this->Dispose();
		windowIsOpened = false;
		return;
	}

	// update cursor style
	SetCursor(LoadCursor(NULL, Mouse::CurrentCursorStyle));

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

	DispatchMessage(&msg);
}

bool UIWindow::IsOpened()
{
	return windowIsOpened;
}

void UIWindow::SetView(UIView* view)
{
	view->DepthSort();
	view->SetSourceWindow(this);

	this->currentView = view;
	for (UIElement* element : view->GetElements())
	{
		element->SetSourceWindow(this);
	}
}

void UIWindow::SetBackgroundColor(Color color)
{
	this->r = color.r; this->g = color.g; this->b = color.b;
	if (ModernWindowCloseButton != nullptr) { ModernWindowCloseButton->SetColor(color); }
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

void UIWindow::mcCreateModernWindow(int width, int height, const char* windowName)
{
	this->SetBorderless(true);
	this->SetBackgroundColor(Color(7, 54, 66, 255));
	this->mcCreateWindow(width, height, windowName);

	// Close Button
	ModernWindowCloseButton = new UIButton();
	ModernWindowCloseButton->SetText("X");
	ModernWindowCloseButton->SetFontName("Arial");
	ModernWindowCloseButton->SetFontSize(14);
	ModernWindowCloseButton->SetTextColor(Color::LightGray);
	ModernWindowCloseButton->SetPosition(this->GetWidth() - 38, 0);
	ModernWindowCloseButton->SetStroke(0);
	ModernWindowCloseButton->SetWidth(38);
	ModernWindowCloseButton->SetHeight(28);
	ModernWindowCloseButton->SetColor(Color(7, 54, 66, 255));
	ModernWindowCloseButton->SetBorderColor(Color(0, 0, 0, 0)); // transparent (invisible border)
	ModernWindowCloseButton->SetHoverOnColor(Color(101, 123, 131, 255));
	ModernWindowCloseButton->SetRoundedCornersRadii(0, 0);
	ModernWindowCloseButton->AddLeftMouseClickEvent(CloseWindowOperation);
	ModernWindowCloseButton->SetSourceWindow(this);

	// Drag Panel
	ModernWindowDragPanel = new UILabel(0, 0, this->GetWidth() - ModernWindowCloseButton->GetWidth(), 32, "", "Verdana", 1, Color(0, 0, 0, 0), 0);
	ModernWindowDragPanel->AddMouseDownEvent(WindowDragPanel_WhileMouseDown);
	ModernWindowDragPanel->AddLeftMouseClickEvent(WindowDragPanel_OnClick);
	ModernWindowDragPanel->SetSourceWindow(this);

	// Window Title
	ModernWindowTitleLabel = new UILabel(20, 0, strlen(windowName) * 14, 30, windowName, "Verdana", 12, Color::White);
	ModernWindowTitleLabel->SetStroke(0);
	ModernWindowTitleLabel->SetTextColor(Color::White);
	ModernWindowTitleLabel->SetTextAllignment(TEXT_ALLIGNMENT_LEFT);
	ModernWindowTitleLabel->SetSourceWindow(this);

	// Adding elements to the static view
	this->staticView->Add(ModernWindowCloseButton);
	this->staticView->Add(ModernWindowTitleLabel);
	this->staticView->Add(ModernWindowDragPanel);
	this->staticView->DepthSort();
	this->staticView->SetSourceWindow(this);
}
