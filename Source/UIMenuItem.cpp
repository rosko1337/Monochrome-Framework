#include "UIMenuItem.h"

UIMenuItem::UIMenuItem(std::string name, int itemID)
{
	this->id = itemID;
	this->name = name;
	this->hMenu = CreateMenu();
}

UIMenuItem::~UIMenuItem()
{
}


