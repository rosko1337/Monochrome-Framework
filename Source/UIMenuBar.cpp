#include "UIMenuBar.h"

UIMenuBar::UIMenuBar()
{
}

UIMenuBar::~UIMenuBar()
{
}

void UIMenuBar::AddMenu(UIMenu* menu)
{
	this->rootMenus.push_back(menu);
}
