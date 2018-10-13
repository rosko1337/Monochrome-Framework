#pragma once

#include <vector>
#include "UIElement.h"

class EventManager;
class UIWindow;

class UIView
{
public:
	UIView();

	// Adds UI element to the view
	void Add(UIElement* element);

	// Removes UI element from the view
	void Remove(UIElement* element);

	// Removes all elements from the view
	void RemoveAllElements();

	// Get index of an element (-1 if it doesn't exist)
	int IndexOf(UIElement* elem);

	// Returns a vector containing all elements in the view
	std::vector<UIElement*>& GetElements() { return this->uiElements; }

	// Assigns all elements in the view their unique Z position from front to back
	void DepthSort();

	// Sets source window
	void SetSourceWindow(UIWindow* window);

	// Returns Z position of the top most element in the view
	int GetTopMostPositionZ() { return this->topMostPositionZ; }

	// Gives an element top most position
	void SetTopMostElement(UIElement* element);

	~UIView();
private:
	std::vector<UIElement*> uiElements;
	EventManager* eventManager;
	UIWindow* sourceWindow;
	int topMostPositionZ = 0;
	void MouseClickEventListener();
	void MouseMovementEventListener();
	void MousePressedEventListener();
};

