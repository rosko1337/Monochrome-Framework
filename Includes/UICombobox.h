#pragma once

#include "UIElement.h"
#include "IUITextElement.h"

class UIScrollPanel;
class UIButton;

class UICombobox : public UIElement, public IUITextElement
{
public:
	UICombobox();
	UICombobox(float xPos, float yPos);
	UICombobox(float xPos, float yPos, float Width, float Height);
	UICombobox(float xPos, float yPos, float Width, float Height, Color color);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Returns current selected index
	int GetSelectedIndex() { return this->SelectedIndex; }

	// Returns whether rounded corners are enabled or not
	bool GetRoundedCorners() { return this->RoundedCorners; }

	// Gets rounded corner X radius
	float GetRoundCornersRadiusX() { return this->roundCornerRadiusX; }

	// Gets rounded corner Y radius
	float GetRoundCornersRadiusY() { return this->roundCornerRadiusY; }

	// Sets current selected index
	void SetSelectedIndex(int index) { this->SelectedIndex = index; }

	// Enables/Disables rounded corners
	void SetRoundedCorners(bool state) { this->RoundedCorners = state; }

	// Sets x and y radii for rounded corners
	void SetRoundedCornersRadii(float radX, float radY) { this->roundCornerRadiusX = radX; this->roundCornerRadiusY = radY; }

	// Returns list of items
	std::vector<std::string> GetItems() { return this->items; }

	// Sets items to a different list
	void SetItems(std::vector<std::string> items);

	// Adds item to the list
	void AddItem(std::string item);

	// Removes item from the list
	void RemoveItem(std::string item);

	// Returns an item at index
	std::string GetItem(int index)
	{
		if (index < 0 || index > items.size()) return NULL;
		return items.at(index);
	}

	// Removes all items
	void Clear();

	// Returns index of an item, -1 if it doesnt exist
	int IndexOf(std::string item);

	// Returns whether combobox is opened or not
	bool IsSelectionOpened() { return this->shouldDrawItems; }

	// Sets whether combobox is opened or not
	void SetSelectionOpened(bool state);

	// Adds an event listener for when the selected index changes
	void AddSelectedIndexChangedEventListener(callback_function callbackFunc);

	~UICombobox();

private:
	void SetupEventListeners();
	int SelectedIndex = 0;
	bool drawItems = false;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;
	std::vector<std::string> items;
	std::vector<UIButton*> itemButtons;
	UIScrollPanel* itemScrollPanel;
	bool shouldDrawItems = false;
	std::string displayedText;
	void trackClickedItems();
};

