#pragma once

#include "UIView.h"

enum MouseClickEventType
{
	LeftClick,
	RightClick
};

enum MouseHoverEventType
{
	HoverOn,
	HoverOff
};

class EventManager
{
public:
	EventManager(UIView* view);

	void ParseMouseClickEvent(POINT point, MouseClickEventType eventType);
	void ParseMouseMovedEvent(POINT point, UIElement*& previousActiveElement);
	void ParseMouseDownEvent(POINT point, bool& mouseAlreadyDownOnElement, UIElement* previousActiveElement, UIElement*& targetElementOut);

	~EventManager();

private:
	UIView* view = nullptr;
	bool Intersect(UIElement* first, UIElement* second);
	bool Intersect(UIElement* elem, POINT point);
	UIElement* GetTopMostElement(std::vector<UIElement*>& targets);

	// Callback function prototype
	typedef void(*callback_function)(UIElement*);

	// different event invocations
	void invokeMouseClickEventCallback(UIElement* sender, MouseClickEventType eventType);
	void invokeMouseHoverEventCallback(UIElement* sender, MouseHoverEventType eventType);
	void invokeMouseDownEventCallback(UIElement* sender);
};

