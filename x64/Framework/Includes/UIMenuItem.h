#pragma once

#include <Windows.h>
#include <string>

class UIWindow;

class UIMenuItem
{
public:
	UIMenuItem(std::wstring name, int itemID);

	HMENU& GetHMenu() { return this->hMenu; }
	std::wstring GetName() { return this->name; }
	int GetID() { return this->id; }

	~UIMenuItem();

private:
	HMENU hMenu;
	int id;
	std::wstring name;
};

