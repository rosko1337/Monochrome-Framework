#include "UIView.h"

UIView::UIView()
{
}

void UIView::Add(UIElement* element)
{
	this->uiElements.push_back(element);
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

UIView::~UIView()
{
}
