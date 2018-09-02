#pragma once

#include "UIElement.h"

class UISlot
{
public:
	UIElement* uiElement = NULL;
	float width = 0, height = 0;
};

class UIScrollPanel : public UIElement
{
public:
	UIScrollPanel();
	UIScrollPanel(float xPos, float yPos);
	UIScrollPanel(float xPos, float yPos, float width, float height);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Fades the elements in over given period of time in miliseconds
	void __stdcall FadeIn(int time)
	{
		for (UISlot* slot : slots)
		{
			slot->uiElement->FadeIn(time);
		}

		this->FadeIn(time);
	}

	// Fades the elements out over given period of time in miliseconds
	void __stdcall FadeOut(int time)
	{
		for (UISlot* slot : slots)
		{
			slot->uiElement->FadeOut(time);
		}

		this->FadeOut(time);
	}

	// Adds element to the next slot
	void AddElement(UIElement* elem);

	// Adds an empty slot
	void AddEmptySlot();

	// Removes element at index from the panel
	void RemoveElement(int index);

	// Removes element from the panel
	void RemoveElement(UIElement* elem);

	// Sets fill of the panel
	void SetFilled(bool state) { this->Filled = state; }

	// Sets width of the container
	void SetWidth(float width)
	{
		this->width = width;
		this->slotWidth = width - (10);
		this->scrollBarPosX = xPos + width - scrollBarWidth;
	}

	// Sets height of the container
	void SetHeight(float height)
	{
		this->height = height;
		this->slotHeight = (height / 10);
		this->scrollBarHeight = height / 4;
		this->scrollBarPosY = yPos;
	}

	// Sets width of the scrollbar
	void SetScrollBarWidth(float width) { this->scrollBarWidth = width; }

	// Returns distance between individual slots
	float GetDistanceBetweenSlots() { return this->distanceBetweenSlots; }

	// Sets distance between individual slots
	void SetDistanceBetweenSlots(float distance) { this->distanceBetweenSlots = distance; }

	// Returns width of a single slot
	float GetSlotWidth() { return this->slotWidth; }

	// Sets width of a single slot
	void SetSlotWidth(float width) { this->slotWidth = width; }

	// Returns height of a single slot
	float GetSlotHeight() { return this->slotHeight; }

	// Sets height of a single slot
	void SetSlotHeight(float height) { this->slotHeight = height; }

	// Returns the distance between the first slot and the top border of the container
	float GetSlotMarginTop() { return this->slotMarginTop; }

	// Returns the distance between any slot left border and left side of the container
	float GetSlotMarginLeft() { return this->slotMarginLeft; }

	// Sets the distance between the first slot and the top border of the container
	void SetSlotMarginTop(float margin) { this->slotMarginTop = margin; }

	// Sets the distance between any slot left border and left side of the container
	void SetSlotMarginLeft(float margin) { this->slotMarginLeft = margin; }

	~UIScrollPanel();

private:
	// Slots and their properties
	std::vector<UISlot*> slots;
	float slotWidth = 180, slotHeight = 30, distanceBetweenSlots = 0;
	float slotMarginTop = 0, slotMarginLeft = 0;
	bool Filled = true;

	// Scroll bars
	bool shouldDrawScrollBar = false;
	float scrollBarWidth = 0, scrollBarHeight = 0;
	float scrollBarPosX = 0, scrollBarPosY = 0;
	void attachScrollBarEventHandler();
	void startScrollBarListener();
	void startScrollWheelListener();
};
