#include "EventManager.h"
#include "UIScrollPanel.h"
#include "UICombobox.h"

template <typename T, class C>
bool isOfType(C* obj)
{
	return dynamic_cast<T*>(obj) != NULL;
}


EventManager::EventManager(UIView* view) : view(view)
{
}

EventManager::~EventManager()
{
}

bool EventManager::Intersect(UIElement* first, UIElement* second)
{
	return false;
}

bool EventManager::Intersect(UIElement* elem, POINT point)
{
	if (point.x < elem->GetPosX() + elem->GetWidth() && point.x > elem->GetPosX() &&
		point.y < elem->GetPosY() + elem->GetHeight() && point.y > elem->GetPosY())
	{
		return true;
	}
	return false;
}

std::vector<UIElement*> EventManager::GetMousePointIntersectionTargets(std::vector<UIElement*>& elements, POINT point)
{
	std::vector<UIElement*> targets;
	for (UIElement* elem : elements)
	{
		if (!elem->IsVisible() || !elem->IsEnabled()) continue;
		if (Intersect(elem, point)) targets.push_back(elem);

		if (isOfType<UIScrollPanel>(elem))
		{
			for (int i = 0; i < static_cast<UIScrollPanel*>(elem)->GetElementCount(); i++)
			{
				if (Intersect(static_cast<UIScrollPanel*>(elem)->GetElement(i), point))
				{
					static_cast<UIScrollPanel*>(elem)->GetElement(i)->SetDepthPositionZ(static_cast<UIScrollPanel*>(elem)->GetDepthPositionZ() + (i + 1));
					targets.push_back(static_cast<UIScrollPanel*>(elem)->GetElement(i));
				}
			}
		}
		else if (isOfType<UICombobox>(elem))
		{
			UIScrollPanel* itemScrollPanel = static_cast<UICombobox*>(elem)->GetItemScrollPanel();
			for (int i = 0; i < itemScrollPanel->GetElementCount(); i++)
			{
				if (Intersect(itemScrollPanel->GetElement(i), point))
				{
					itemScrollPanel->GetElement(i)->SetDepthPositionZ(itemScrollPanel->GetDepthPositionZ() + (i + 1));
					targets.push_back(itemScrollPanel->GetElement(i));
				}
			}
		}
	}
	return targets;
}

UIElement* EventManager::GetTopMostElement(std::vector<UIElement*>& targets)
{
	UIElement* targetElement = nullptr;
	if (targets.size() == 1) { targetElement = targets.at(0); }
	else if (targets.size() > 1)
	{
		// elements are overlapping
		int topPosZ = -1;
		for (UIElement* target : targets)
		{
			if (target->GetDepthPositionZ() > topPosZ)
			{
				targetElement = target;
				topPosZ = target->GetDepthPositionZ();
			}
		}
	}

	return targetElement;
}

void EventManager::invokeMouseClickEventCallback(UIElement* sender, MouseClickEventType eventType)
{
	if (eventType == LeftClick)
		for (callback_function func : sender->GetLeftMouseClickEvents())
			func(sender);

	if (eventType == RightClick)
		for (callback_function func : sender->GetRightMouseClickEvents())
			func(sender);
}

void EventManager::invokeMouseHoverEventCallback(UIElement* sender, MouseHoverEventType eventType)
{
	if (eventType == HoverOn)
		for (callback_function func : sender->GetMouseHoverOnEvents())
			func(sender);

	if (eventType == HoverOff)
		for (callback_function func : sender->GetMouseHoverOffEvents())
			func(sender);
}

void EventManager::invokeMouseDownEventCallback(UIElement * sender)
{
	for (callback_function func : sender->GetMouseDownEvents())
		func(sender);
}

void EventManager::ParseMouseClickEvent(POINT point, MouseClickEventType eventType)
{
	std::vector<UIElement*> targets = GetMousePointIntersectionTargets(view->GetElements(), point);

	if (targets.size() == 0) return;

	if (targets.size() == 1)
	{
		// immidiately call the callback function
		invokeMouseClickEventCallback(targets.at(0), eventType);
	}
	else if (targets.size() > 1)
	{
		// elements are overlapping
		int topPosZ = -1;
		UIElement* targetElement = nullptr;
		for (UIElement* target : targets)
		{
			if (target->GetDepthPositionZ() > topPosZ)
			{
				targetElement = target;
				topPosZ = target->GetDepthPositionZ();
			}
		}

		if (targetElement != nullptr)
		{
			invokeMouseClickEventCallback(targetElement, eventType);
		}
	}
}

void EventManager::ParseMouseMovedEvent(POINT point, UIElement*& previousActiveElement)
{
	std::vector<UIElement*> targets = GetMousePointIntersectionTargets(view->GetElements(), point);

	UIElement* targetElement = GetTopMostElement(targets);

	// detecting hover on/off events
	if (targetElement != previousActiveElement)
	{
		if (previousActiveElement != nullptr)
		{
			// hover off the previous element
			invokeMouseHoverEventCallback(previousActiveElement, HoverOff);
		}

		if (targetElement != nullptr)
		{
			// hover on the new element
			invokeMouseHoverEventCallback(targetElement, HoverOn);
		}
	}

	// update previous element
	previousActiveElement = targetElement;
}

void EventManager::ParseMouseDownEvent(POINT point, bool& mouseAlreadyDownOnElement, UIElement* previousActiveElement, UIElement*& targetElementOut)
{
	std::vector<UIElement*> targets = GetMousePointIntersectionTargets(view->GetElements(), point);

	UIElement* targetElement = GetTopMostElement(targets);
	if (targetElement == previousActiveElement && targetElement != nullptr && mouseAlreadyDownOnElement)
	{
		// call the callback function
		invokeMouseDownEventCallback(targetElement);
	}

	if (targetElement != previousActiveElement)
		mouseAlreadyDownOnElement = false;

	targetElementOut = targetElement;
}
