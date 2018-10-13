#pragma once

#include "Graphics.h"
#include "Color.h"
#include "UIWindow.h"
#include <thread>

#define TEXT_ALLIGNMENT_CENTER 1
#define TEXT_ALLIGNMENT_LEFT 2
#define TEXT_ALLIGNMENT_RIGHT 3

class MouseClickEventListener;
class MouseRightClickEventListener;
class MouseDownEventListener;
class MouseHoverOnEventListener;
class MouseHoverOffEventListener;

class UIElement
{
public:
	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	virtual void __stdcall Draw(Graphics* graphics) = 0;

	// Gets source window for the element's event listeners
	UIWindow* GetSourceWindow() { return this->SourceWindow; }

	// Sets source window for the element's event listeners
	void SetSourceWindow(UIWindow* srcWindow) { this->SourceWindow = srcWindow; }

	// Get enabled state of the element
	virtual bool IsEnabled() { return this->enabled; }

	// Get visible state of the element
	virtual bool IsVisible() { return this->visible; }

	// Get color of the element
	virtual Color GetColor() { return this->color; }

	// Get width of the element
	virtual float GetWidth() { return this->width; }

	// Get height of the element
	virtual float GetHeight() { return this->height; }

	// Get X coordinate of the element’s position
	virtual float GetPosX() { return this->xPos; }

	// Get Y coordinate of the element’s position
	virtual float GetPosY() { return this->yPos; }

	// Returns layer position of the element
	int GetDepthPositionZ() { return this->zPos; }

	// Sets the layer position of the element
	void SetDepthPositionZ(int zPos) { this->zPos = zPos; }

	// Set enabled state of the element
	virtual void SetEnabled(bool state)
	{
		this->enabled = state;
		if (state) { this->color.a = normalAlpha; } // reset alpha component back to original
		else { this->color.a = 100; }				// make the whole element darker by lowering the alpha component
	}

	// Set visible state of the element
	virtual void SetVisible(bool state) { this->visible = state; }

	// Set color of the element
	virtual void SetColor(Color color) { this->color = color; this->normalAlpha = color.a; }

	// Set position of the element
	virtual void SetPosition(float x, float y) { this->xPos = x; this->yPos = y; }

	// Set X component of element’s position
	virtual void SetPosX(float x) { this->xPos = x; }

	// Set Y component of element’s position
	virtual void SetPosY(float y) { this->yPos = y; }

	// Set width and height of the element
	virtual void SetSize(float width, float height) { this->width = width; this->height = height; }

	// Set width of the element
	virtual void SetWidth(float width) { this->width = width; }

	// Set height of the element
	virtual void SetHeight(float height) { this->height = height; }
	
	// Callback function prototype
	typedef void(*callback_function)(UIElement*);

	// Fades the element out over given period of time in miliseconds
	virtual void __stdcall FadeOut(int time)
	{
		std::thread fadeout_thread([this, &time]
		{
			Color currentColor = GetColor();
			int currentAlpha = currentColor.a;
			if (currentAlpha == 0) { currentAlpha = 1; }
			int sleepIntervals = time / currentAlpha;
			for (int i = currentAlpha; i > 0; i--)
			{
				currentColor.a--;
				this->SetColor(currentColor);
				Sleep(sleepIntervals);
			}
			this->SetVisible(false);
		});
		fadeout_thread.detach();
	}

	// Fades the element in over given period of time in miliseconds until the needed alpha value is reached
	virtual void __stdcall FadeIn(int time, int finalAlpha = 255)
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
				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

	// Callback function for left mouse click event
	virtual void AddLeftMouseClickEvent(callback_function event) { this->leftMouseClickEvents.push_back(event); }

	// Returns callback functions for left mouse click event
	virtual std::vector<callback_function>& GetLeftMouseClickEvents() { return this->leftMouseClickEvents; }

	// Callback function for right mouse click event
	virtual void AddRightMouseClickEvent(callback_function event) { this->rightMouseClickEvents.push_back(event); }

	// Returns callback functions for right mouse click event
	virtual std::vector<callback_function>& GetRightMouseClickEvents() { return this->rightMouseClickEvents; }

	// Callback function for mouse hover on event
	virtual void AddMouseHoverOnEvent(callback_function event) { this->mouseHoverOnEvents.push_back(event); }

	// Returns callback functions for mouse hover on event
	virtual std::vector<callback_function>& GetMouseHoverOnEvents() { return this->mouseHoverOnEvents; }

	// Callback function for mouse hover off event
	virtual void AddMouseHoverOffEvent(callback_function event) { this->mouseHoverOffEvents.push_back(event); }

	// Returns callback functions for mouse hover off event
	virtual std::vector<callback_function>& GetMouseHoverOffEvents() { return this->mouseHoverOffEvents; }

	// Callback function for mouse down event
	virtual void AddMouseDownEvent(callback_function event) { this->mouseDownEvents.push_back(event); }

	// Returns callback functions for mouse down event
	virtual std::vector<callback_function>& GetMouseDownEvents() { return this->mouseDownEvents; }

	// Public virtual destructor
	virtual ~UIElement() {  }

protected:
	UIElement() { /* default constructor */ }
	UIElement(float x, float y) { this->xPos = x; this->yPos = y; }
	UIElement(float x, float y, float width, float height) { this->xPos = x; this->yPos = y; this->width = width; this->height = height; }
	UIElement(float x, float y, float width, float height, Color color) { this->xPos = x; this->yPos = y; this->width = width; this->height = height; this->color = color; this->normalAlpha = color.a; }

	UIWindow* SourceWindow = 0;

	/* Universal Attributes */
	bool enabled = true;
	bool visible = true;
	Color color = Color::LightGray;
	int normalAlpha = 255; // max alpha
	float width = 20;
	float height = 20;
	float xPos = 20;
	float yPos = 20;
	int zPos = 0;

	// list of callback events
	std::vector<callback_function> leftMouseClickEvents;
	std::vector<callback_function> rightMouseClickEvents;
	std::vector<callback_function> mouseHoverOnEvents;
	std::vector<callback_function> mouseHoverOffEvents;
	std::vector<callback_function> mouseDownEvents;

	// Creates proper d2d1 text allignment options
	void MakeTextAllignment(int allignment, DWRITE_TEXT_ALIGNMENT& textAllignment, DWRITE_PARAGRAPH_ALIGNMENT& paragraphAllignment)
	{
		if (allignment == TEXT_ALLIGNMENT_CENTER)
		{
			textAllignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			paragraphAllignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		else if (allignment == TEXT_ALLIGNMENT_LEFT)
		{
			textAllignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			paragraphAllignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}
		else if (allignment == TEXT_ALLIGNMENT_RIGHT)
		{
			textAllignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			paragraphAllignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
		}
		else
		{
			textAllignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			paragraphAllignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
	}
};