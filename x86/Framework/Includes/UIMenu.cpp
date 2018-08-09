#include "UIMenu.h"

UIMenu::UIMenu(std::string name)
{
	this->name = name;
	hMenu = CreateMenu();
}

UIMenu::~UIMenu()
{
}

void UIMenu::AddMenu(UIMenu* menu)
{
	AppendMenuA(this->hMenu, MF_POPUP, (UINT_PTR)menu->GetHMenu(), menu->GetName().c_str());
}

void UIMenu::AddSeparator()
{
	AppendMenuA(this->hMenu, MF_SEPARATOR, NULL, NULL);
}

void UIMenu::AddMenuItem(UIMenuItem* menuItem)
{
	AppendMenuA(this->hMenu, MF_STRING, menuItem->GetID(), menuItem->GetName().c_str());
}
