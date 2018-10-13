#include "UIView.h"
#include "EventManager.h"
#include "Mouse.h"
#include "UIWindow.h"
#include "UIScrollPanel.h"
#include "UICombobox.h"
#include <thread>

template <typename T, class C>
bool isOfType(C* obj)
{
	return dynamic_cast<T*>(obj) != NULL;
}

UIView::UIView()
{
	this->eventManager = new EventManager(this);
	std::thread mouseClickEventListenerThread(&UIView::MouseClickEventListener, this); // mouse clicks
	mouseClickEventListenerThread.detach();

	std::thread mouseMovementEventListenerThread(&UIView::MouseMovementEventListener, this); // mouse movement
	mouseMovementEventListenerThread.detach();

	std::thread mousePressedEventListenerThread(&UIView::MousePressedEventListener, this); // mouse pressed state
	mousePressedEventListenerThread.detach();
}

void UIView::Add(UIElement* element)
{
	this->uiElements.push_back(element);
	if (isOfType<UIScrollPanel>(element))
	{
		for (int i = 0; i < static_cast<UIScrollPanel*>(element)->GetElementCount(); i++)
		{
			Add(static_cast<UIScrollPanel*>(element)->GetElement(i));
		}
	}
}

void UIView::Remove(UIElement* element)
{
	for (int i = 0; i < this->uiElements.size(); i++)
	{
		if (this->uiElements.at(i) == element)
		{
			this->uiElements.erase(this->uiElements.begin() + (i));
		}
	}
}

void UIView::RemoveAllElements()
{
	this->uiElements.clear();
}

int UIView::IndexOf(UIElement* elem)
{
	auto index = std::find(uiElements.begin(), uiElements.end(), elem) - uiElements.begin();
	if (index >= uiElements.size()) return -1;
	return index;
}

void UIView::DepthSort()
{
	int depth = 0;
	for (UIElement* elem : uiElements)
	{
		elem->SetDepthPositionZ(depth);
		depth++;
	}
	topMostPositionZ = depth;
}

void UIView::SetTopMostElement(UIElement* element)
{
	int currentIndex = IndexOf(element);
	if (currentIndex != -1)
	{
		this->uiElements.erase(this->uiElements.begin() + (currentIndex));
	}

	this->uiElements.push_back(element);

	DepthSort();
}

UIView::~UIView()
{
}

void UIView::SetSourceWindow(UIWindow* window)
{
	this->sourceWindow = window;
}

void UIView::MouseClickEventListener()
{
	bool lmClicked = false;
	bool rmClicked = false;

	while (true)
	{
		Sleep(2);
		if (sourceWindow == nullptr) continue;

		// checking if buttons are not clicked anymore
		if(GetAsyncKeyState(VK_LBUTTON) == 0) lmClicked = false;
		if(GetAsyncKeyState(VK_RBUTTON) == 0) rmClicked = false;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && !lmClicked)
		{
			lmClicked = true;
			POINT pos;
			pos.x = Mouse::GetMousePosX(sourceWindow);
			pos.y = Mouse::GetMousePosY(sourceWindow);
			if (pos.x > sourceWindow->GetWidth() || pos.x < 0 || pos.y > sourceWindow->GetHeight() || pos.y < 0) continue;

			eventManager->ParseMouseClickEvent(pos, LeftClick);
		}

		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !rmClicked)
		{
			rmClicked = true;
			POINT pos;
			pos.x = Mouse::GetMousePosX(sourceWindow);
			pos.y = Mouse::GetMousePosY(sourceWindow);
			if (pos.x > sourceWindow->GetWidth() || pos.x < 0 || pos.y > sourceWindow->GetHeight() || pos.y < 0) continue;

			eventManager->ParseMouseClickEvent(pos, RightClick);
		}
	}
}

void UIView::MouseMovementEventListener()
{
	POINT previousMousePosition;
	previousMousePosition.x = Mouse::GetMousePosX(sourceWindow);
	previousMousePosition.y = Mouse::GetMousePosY(sourceWindow);

	// hover on/off events
	UIElement* previousActiveElement = nullptr;

	while (true)
	{
		Sleep(2);
		if (sourceWindow == nullptr) continue;

		POINT currentMousePosition;
		currentMousePosition.x = Mouse::GetMousePosX(sourceWindow);
		currentMousePosition.y = Mouse::GetMousePosY(sourceWindow);
		// if mouse is not in the window
		if (currentMousePosition.x > sourceWindow->GetWidth() || currentMousePosition.x < 0 ||  currentMousePosition.y > sourceWindow->GetHeight() || currentMousePosition.y < 0) continue;

		if (currentMousePosition.x != previousMousePosition.x || currentMousePosition.y != previousMousePosition.y)
		{
			// mouse moved
			eventManager->ParseMouseMovedEvent(currentMousePosition, previousActiveElement);
		}

		previousMousePosition = currentMousePosition;
	}
}

void UIView::MousePressedEventListener()
{
	bool mouseDown = false;
	bool mouseAlreadyDownOnElement = false;
	UIElement* previousActiveElement = nullptr;

	while (true)
	{
		Sleep(2);
		if (sourceWindow == nullptr) continue;

		// checking if mouse is not pressed anymore
		if (GetAsyncKeyState(VK_LBUTTON) == 0)
		{
			mouseDown = false;
			mouseAlreadyDownOnElement = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			POINT pos;
			pos.x = Mouse::GetMousePosX(sourceWindow);
			pos.y = Mouse::GetMousePosY(sourceWindow);
			if (pos.x > sourceWindow->GetWidth() || pos.x < 0 || pos.y > sourceWindow->GetHeight() || pos.y < 0) continue;

			UIElement* targetElement = nullptr;
			eventManager->ParseMouseDownEvent(pos, mouseAlreadyDownOnElement, previousActiveElement, targetElement);
			
			if (!mouseDown)
			{
				previousActiveElement = targetElement;
				mouseAlreadyDownOnElement = true;
			}

			mouseDown = true;
		}
	}
}
