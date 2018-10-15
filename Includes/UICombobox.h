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

	// Returns the color of the drop-down arrow symbol
	Color GetArrowColor() { return this->arrowColor; }

	// Sets the color of the drop-down arrow symbol
	void SetArrowColor(Color color) { this->arrowColor = color; }

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

	// Sets enabled state of the checkbox
	void SetEnabled(bool state)
	{
		this->enabled = state;
		if (state)
		{
			color.a = normalAlpha;
			textColor.a = normalAlpha;
			arrowColor.a = normalAlpha;
		}
		else
		{
			color.a = 36;
			textColor.a = 36;
			arrowColor.a = 36;
		}
	}

	// Fades the element out over given period of time in miliseconds
	void __stdcall FadeOut(int time)
	{
		std::thread fadeout_thread([this, &time]
		{
			Color currentColor = GetColor();
			float currentAlpha = currentColor.a;
			if (currentAlpha == 0) { currentAlpha = 1; }
			int sleepIntervals = time / (int)currentAlpha;
			for (int i = currentAlpha; i > 0; i--)
			{
				currentColor.a--;
				this->SetColor(currentColor);

				Color newTextColor = GetTextColor();
				newTextColor.a = currentColor.a;
				arrowColor.a = currentColor.a;
				this->SetTextColor(newTextColor);

				Sleep(sleepIntervals);
			}
			this->SetVisible(false);
			//EmergencySetHoverOffColor();
		});
		fadeout_thread.join();
	}

	// Fades the element in over given period of time in miliseconds until the needed alpha value is reached
	void __stdcall FadeIn(int time, int finalAlpha = 255)
	{
		int sleepInterval = 0;
		if (finalAlpha < color.a) return; // invalid alpha state
		int delta = finalAlpha - color.a;
		if (delta == 0)
		{
			sleepInterval = time;
		}
		else
		{
			sleepInterval = time / delta;
		}

		if (finalAlpha >= 255)
			finalAlpha = 254;

		std::thread fadein_thread([this, sleepInterval, finalAlpha] {
			this->SetVisible(true);
			for (int i = color.a; i <= finalAlpha; i++)
			{
				Sleep(sleepInterval);
				uint8_t val = (uint8_t)i;
				color.a = val;


				Color newTextColor = GetTextColor();
				newTextColor.a = color.a;
				arrowColor.a = color.a;
				this->SetTextColor(newTextColor);

				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

	// Returns the address of the scroll panel containing the visual buttons for each item in the combobox
	UIScrollPanel* GetItemScrollPanel() { return this->itemScrollPanel; }

	~UICombobox();

private:
	void SetupEventListeners();
	int SelectedIndex = 0;
	bool drawItems = false;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;
	Color arrowColor = Color::Black;
	std::vector<std::string> items;
	std::vector<UIButton*> itemButtons;
	UIScrollPanel* itemScrollPanel;
	bool shouldDrawItems = false;
	std::string displayedText;
	void trackClickedItems();
};

