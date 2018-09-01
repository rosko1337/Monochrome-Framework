#pragma once

#include <Windows.h>
#include <string>

class UIWindow;

class UIMenuItem
{
public:
	UIMenuItem(std::string name, int itemID);

	HMENU& GetHMenu() { return this->hMenu; }
	std::string GetName() { return this->name; }
	int GetID() { return this->id; }

	~UIMenuItem();

private:
	HMENU hMenu;
	int id;
	std::string name;
};

