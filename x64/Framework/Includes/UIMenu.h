#pragma once

#include "UIMenuItem.h"

class UIMenu
{
public:
	UIMenu(std::wstring name);

	HMENU& GetHMenu() { return this->hMenu; }
	std::wstring GetName() { return this->name; }

	void AddMenu(UIMenu* menu);
	void AddSeparator();
	void AddMenuItem(UIMenuItem* menuItem);

	~UIMenu();

private:
	HMENU hMenu;
	std::wstring name;
};

