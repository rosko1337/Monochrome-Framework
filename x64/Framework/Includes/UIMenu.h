#pragma once

#include "UIMenuItem.h"

class UIMenu
{
public:
	UIMenu(std::string name);

	HMENU& GetHMenu() { return this->hMenu; }
	std::string GetName() { return this->name; }

	void AddMenu(UIMenu* menu);
	void AddSeparator();
	void AddMenuItem(UIMenuItem* menuItem);

	~UIMenu();

private:
	HMENU hMenu;
	std::string name;
};

