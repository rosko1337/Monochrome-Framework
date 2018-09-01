#pragma once

#include <vector>
#include "UIElement.h"

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

	// Returns a vector containing all elements in the view
	std::vector<UIElement*>& GetElements() { return this->uiElements; }

	~UIView();
private:
	std::vector<UIElement*> uiElements;
};

