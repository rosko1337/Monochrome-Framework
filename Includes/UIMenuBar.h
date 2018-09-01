#pragma once

#include "UIMenu.h"
#include <vector>

class UIMenuBar
{
public:
	UIMenuBar();

	void AddMenu(UIMenu* menu);
	std::vector<UIMenu*>& GetMenuList() { return this->rootMenus; }

	~UIMenuBar();

private:
	std::vector<UIMenu*> rootMenus;
};

