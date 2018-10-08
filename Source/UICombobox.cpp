#include "UICombobox.h"
#include "UIScrollPanel.h"
#include "UIButton.h"
#include "MouseClickEventListener.h"
#include "MouseClickAwayEventListener.h"
#include "MouseHoverOnEventListener.h"
#include "MouseHoverOffEventListener.h"

void Combobox_OnClick(UIElement* sender)
{
	UICombobox* combobox = static_cast<UICombobox*>(sender);
	combobox->SetSelectionOpened(true);
}

void Combobox_OnClickAway(UIElement* sender)
{
	UICombobox* combobox = static_cast<UICombobox*>(sender);
	combobox->SetSelectionOpened(false);
}

void Combobox_HoverOn(UIElement* sender)
{
	UICombobox* combobox = static_cast<UICombobox*>(sender);
}

void Combobox_HoverOff(UIElement* sender)
{
	UICombobox* combobox = static_cast<UICombobox*>(sender);
}

UICombobox::UICombobox()
{
	itemScrollPanel = new UIScrollPanel();
	width = 120;
	itemScrollPanel->SetWidth(width);
	itemScrollPanel->SetHeight(height * 3);
	itemScrollPanel->SetSlotHeight(height);
	SetupEventListeners();
}

UICombobox::UICombobox(float xPos, float yPos) : UIElement(xPos, yPos)
{
	itemScrollPanel = new UIScrollPanel();
	width = 120;
	itemScrollPanel->SetWidth(width);
	itemScrollPanel->SetHeight(height * 3);
	itemScrollPanel->SetSlotHeight(height);
	SetupEventListeners();
}

UICombobox::UICombobox(float xPos, float yPos, float Width, float Height) : UIElement(xPos, yPos, Width, Height)
{
	itemScrollPanel = new UIScrollPanel();
	itemScrollPanel->SetWidth(Width);
	itemScrollPanel->SetHeight(Height * 3);
	itemScrollPanel->SetSlotHeight(Height);
	SetupEventListeners();
}

UICombobox::UICombobox(float xPos, float yPos, float Width, float Height, Color color) : UIElement(xPos, yPos, Width, Height, color)
{
	itemScrollPanel = new UIScrollPanel();
	itemScrollPanel->SetWidth(Width);
	itemScrollPanel->SetHeight(Height * 3);
	itemScrollPanel->SetSlotHeight(Height);
	SetupEventListeners();
}

void UICombobox::SetItems(std::vector<std::string> items)
{
	// Removing existing items
	this->Clear();

	// Adding new items
	for (std::string newItem : items)
	{
		AddItem(newItem);
	}
}

void UICombobox::AddItem(std::string item)
{
	this->items.push_back(item);

	// add button representing the item
	UIButton* itemBtn = new UIButton(0, 0, 0, 0, item, FontName, FontSize, Color::LightGray);
	itemBtn->HideBorder();
	itemScrollPanel->AddElement(itemBtn);
}

void UICombobox::RemoveItem(std::string item)
{
	int itemIndex = IndexOf(item);
	if (itemIndex == -1) return; // item doesnt exist
	itemScrollPanel->RemoveElement(itemIndex);
	items.erase(items.begin() + itemIndex);
}

void UICombobox::Clear()
{
	// Removing existing items
	items.clear();
	itemScrollPanel->RemoveAllElements();
}

int UICombobox::IndexOf(std::string item)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items.at(i)._Equal(item))
		{
			return i;
		}
	}
	return -1;
}

void UICombobox::SetupEventListeners()
{
	new MouseClickEventListener(this, Combobox_OnClick);			// opens selection of items
	new MouseClickAwayEventListener(this, Combobox_OnClickAway);	// closes selection of items
	new MouseHoverOnEventListener(this, Combobox_HoverOn);			// highlights the combobox 
	new MouseHoverOffEventListener(this, Combobox_HoverOff);		// stops highlighting the combobox 
	std::thread itemClickThread(&UICombobox::trackClickedItems, this);
	itemClickThread.detach();										// detects clicking on elements
}

void UICombobox::SetSelectionOpened(bool state)
{
	if (state)
	{
		// open selection
		this->shouldDrawItems = true;
	}
	else
	{
		// close selection if mouse is not on the selection area
		float mouseX = Mouse::GetMousePosX(GetSourceWindow()), mouseY = Mouse::GetMousePosY(GetSourceWindow());

		bool mouseInsideSelectionArea = mouseX < itemScrollPanel->GetPosX() + itemScrollPanel->GetWidth() && mouseX > itemScrollPanel->GetPosX() &&
			mouseY < itemScrollPanel->GetPosY() + itemScrollPanel->GetHeight() && mouseY > itemScrollPanel->GetPosY();
		if (!mouseInsideSelectionArea)
		{
			this->shouldDrawItems = false;
		}
	}
}
UICombobox::~UICombobox()
{
}

void UICombobox::Draw(Graphics* graphics)
{
	if (!RoundedCorners)
	{
		graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, 0, true); // frame
	}
	else
	{
		graphics->DrawRoundedRectangle(xPos, yPos, width, height,
			roundCornerRadiusX, roundCornerRadiusY, color.r, color.g, color.b, color.a, 0, true); // frame
	}

	graphics->drawText(L"▼", "Arial", 16, FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL, xPos + width - 14.0f, yPos, 4.0f, height - 0.6f,
		0, 0, 0, 255, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // Arrow Key Down


	if (items.size() < 1) return;

	// drawing selected item text
	graphics->drawText(items.at(SelectedIndex), FontName, FontSize, fontStyle, fontWeight, xPos + 2.0f, yPos, width - 14.0f, height,
		0, 0, 0, 255, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // Actual Text)

	// drawing the scroll panel with items
	if (shouldDrawItems)
	{
		itemScrollPanel->SetPosition(xPos, yPos + height);
		itemScrollPanel->SetSourceWindow(GetSourceWindow());
		itemScrollPanel->Draw(graphics);
	}
}

void UICombobox::trackClickedItems()
{
	while (true)
	{
		Sleep(10);
		if (shouldDrawItems)
		{
			// Get Mouse Position
			POINT pos;
			pos.x = Mouse::GetMousePosX(GetSourceWindow());
			pos.y = Mouse::GetMousePosY(GetSourceWindow());

			// If Mouse is Pressed
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				for (int i = 0; i < itemScrollPanel->GetElementCount(); i++)
				{
					UIElement* elem = itemScrollPanel->GetElement(i);
					if (pos.x < elem->GetPosX() + elem->GetWidth() && pos.x > elem->GetPosX() &&
						pos.y < elem->GetPosY() + elem->GetHeight() && pos.y > elem->GetPosY())
					{
						this->SelectedIndex = i;
						this->Text = items.at(SelectedIndex);
						this->shouldDrawItems = false;
					}
				}
			}
		}
	}
}

void UICombobox::AddSelectedIndexChangedEventListener(callback_function callbackFunc)
{
	std::thread callbackThread([this, callbackFunc]
	{
		int previousSelectedIndex = SelectedIndex;
		while (true)
		{
			Sleep(14);
			if (visible && enabled && GetSourceWindow() != nullptr)
			{
				int currentSelectedIndex = SelectedIndex;
				if (previousSelectedIndex != currentSelectedIndex)
				{
					callbackFunc(this);
				}
				previousSelectedIndex = currentSelectedIndex;
			}
		}
	});
	callbackThread.detach();
}
