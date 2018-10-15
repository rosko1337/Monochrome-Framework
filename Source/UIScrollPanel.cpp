#include "UIScrollPanel.h"
#include "Mouse.h"
#include "UIView.h"

UIScrollPanel::UIScrollPanel()
{
	this->scrollBarWidth = 14;

	this->slotWidth = width - (10);
	this->slotHeight = (height / 10);

	// Scroll Bar
	this->scrollBarWidth = 14;
	this->scrollBarHeight = height / 4;
	this->scrollBarPosX = xPos + width - scrollBarWidth;
	this->scrollBarPosY = yPos;

	// Start event handlers
	attachScrollBarEventHandler();
}

UIScrollPanel::UIScrollPanel(float xPos, float yPos) : UIElement(xPos, yPos)
{
	this->scrollBarWidth = 14;

	this->slotWidth = width - (10);
	this->slotHeight = (height / 10);

	// Scroll Bar
	this->scrollBarWidth = 14;
	this->scrollBarHeight = height / 4;
	this->scrollBarPosX = xPos + width - scrollBarWidth;
	this->scrollBarPosY = yPos;

	// Start event handlers
	attachScrollBarEventHandler();
}

UIScrollPanel::UIScrollPanel(float xPos, float yPos, float width, float height) : UIElement(xPos, yPos, width, height)
{
	this->slotWidth = width - (10);
	this->slotHeight = (height / 10);

	// Scroll Bar
	this->scrollBarWidth = 14;
	this->scrollBarHeight = height / 4;
	this->scrollBarPosX = xPos + width - scrollBarWidth;
	this->scrollBarPosY = yPos;

	// Start event handlers
	attachScrollBarEventHandler();
}

UIScrollPanel::~UIScrollPanel()
{
}

void __stdcall UIScrollPanel::Draw(Graphics* graphics)
{
	graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, 0, Filled);

	// Calculating the start and end indices of vector to draw elements from
	float topSpaceFromScrollBar = scrollBarPosY - this->yPos;
	float scrolledPercent = topSpaceFromScrollBar / (this->height - scrollBarHeight);
	int numberOfElementsVisible = (int)((this->height - slotMarginTop + distanceBetweenSlots) / (slotHeight + distanceBetweenSlots));
	if (numberOfElementsVisible > slots.size())
	{
		numberOfElementsVisible = (int)slots.size();
	}
	int startIndex = (int)(scrolledPercent * (slots.size() - numberOfElementsVisible));

	for (int i = startIndex; i < (int)slots.size(); i++)
	{
		if (slots.size() > numberOfElementsVisible)
		{
			shouldDrawScrollBar = true;
			if (i == startIndex + numberOfElementsVisible)
			{
				break;
			}
		}
		else
		{
			shouldDrawScrollBar = false;
		}

		// Main calculation for position of a slot visible on the screen
		float slotPosX = this->xPos + slotMarginLeft;
		float slotPosY = this->yPos + slotMarginTop + ((i - startIndex) * slotHeight) + ((i - startIndex) * distanceBetweenSlots);

		if (slots.at(i)->uiElement != nullptr)
		{
			slots.at(i)->uiElement->SetPosition(slotPosX, slotPosY);
			slots.at(i)->uiElement->SetSourceWindow(GetSourceWindow());
			slots.at(i)->uiElement->Draw(graphics);
		}
	}

	if (shouldDrawScrollBar)
	{
		// Resize the scrollbar according to the contents
		float heightOfAllElements = ((slots.size() * slotHeight) + (slots.size() * distanceBetweenSlots)) - distanceBetweenSlots + slotMarginTop + slotHeight;
		float heightOfElementsShown = this->height - slotMarginTop;
		float scrollBarHeightPercentage = heightOfElementsShown / heightOfAllElements;
		this->scrollBarHeight = this->height * scrollBarHeightPercentage;

		// Adjust scroll bar position to keep it in bounds
		if (this->scrollBarPosY < this->yPos)
		{
			this->scrollBarPosY = this->yPos;
		}
		else if (scrollBarPosY + scrollBarHeight > this->yPos + this->height)
		{
			this->scrollBarPosY = (this->yPos + this->height) - scrollBarHeight;
		}

		// Draw Sroll Bar (Vertical)
		Color sbColor = Color::DarkGray;
		graphics->DrawRectangle(scrollBarPosX, scrollBarPosY, scrollBarWidth, scrollBarHeight,
			sbColor.r, sbColor.g, sbColor.b, sbColor.a, 0, true);
	}
}

void UIScrollPanel::AddElement(UIElement* elem)
{
	UISlot* slot = new UISlot();
	slot->width = slotWidth;
	slot->height = slotHeight;

	elem->SetWidth(slot->width);
	elem->SetHeight(slot->height);
	slot->uiElement = elem;

	this->slots.push_back(slot);
}

void UIScrollPanel::AddEmptySlot()
{
	UISlot* slot = new UISlot();
	slot->width = slotWidth;
	slot->height = slotHeight;
	// No UIElement will be added to this slot therefore creating an empty space
	this->slots.push_back(slot);
}

void UIScrollPanel::RemoveElement(int index)
{
	if (index < slots.size() && index >= 0 && slots.size() > 0)
	{
		slots.erase(slots.begin() + index);
	}
}

void UIScrollPanel::RemoveElement(UIElement* elem)
{
	for (int i = 0; i < (int)slots.size(); i++)
	{
		if (slots.at(i)->uiElement == elem)
		{
			slots.erase(slots.begin() + i);
			break;
		}
	}
}

int UIScrollPanel::IndexOf(UIElement* elem)
{
	for (int i = 0; i < slots.size(); i++)
	{
		if (slots.at(i)->uiElement == elem)
		{
			return i;
		}
	}
	return -1;
}

// Attach scroll bar listener
void UIScrollPanel::attachScrollBarEventHandler()
{
	std::thread scrollBar_thread(&UIScrollPanel::startScrollBarListener, this);
	scrollBar_thread.detach();

	std::thread scrollWheel_thread(&UIScrollPanel::startScrollWheelListener, this);
	scrollWheel_thread.detach();
}

// Start scroll bar listener loop
void UIScrollPanel::startScrollBarListener()
{
	POINT previousPos;
	previousPos.x = Mouse::GetMousePosX(GetSourceWindow());
	previousPos.y = Mouse::GetMousePosY(GetSourceWindow());

	bool mouseDownOnBar = false;
	bool mouseScrolling = false;
	while (true)
	{
		Sleep(10);
		if (shouldDrawScrollBar)
		{
			if (GetAsyncKeyState(VK_LBUTTON) == 0) // Mouse isn't pressed 
			{
				mouseScrolling = false;
				mouseDownOnBar = false;
				continue;
			}

			// Get Mouse Position
			POINT pos;
			pos.x = Mouse::GetMousePosX(GetSourceWindow());
			pos.y = Mouse::GetMousePosY(GetSourceWindow());

			// If Mouse is Pressed
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{

				if (pos.x < this->scrollBarPosX + this->scrollBarWidth && pos.x > this->scrollBarPosX &&
					pos.y < this->scrollBarPosY + this->scrollBarHeight && pos.y > this->scrollBarPosY)
				{
					mouseDownOnBar = true;
				}
			}

			// Check for scrolling if mouse is pressed on the scroll bar
			if (mouseDownOnBar)
			{
				if (!mouseScrolling)
				{
					mouseScrolling = true;
					previousPos = pos;
					continue;
				}
				else
				{
					float distanceMoved = (float)(pos.y - previousPos.y);
					this->scrollBarPosY += distanceMoved;

					// Keep scrollbar in slot area's bounds
					if (this->scrollBarPosY < this->yPos)
					{
						this->scrollBarPosY = this->yPos;
					}
					else if (scrollBarPosY + scrollBarHeight > this->yPos + this->height)
					{
						this->scrollBarPosY = (this->yPos + this->height) - scrollBarHeight;
					}
				}
			}

			// Update previous position
			previousPos = pos;
		}
	}
}

void UIScrollPanel::startScrollWheelListener()
{
	while (true)
	{
		Sleep(10);

	}
}
