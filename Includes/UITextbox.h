#pragma once

#include "IUITextElement.h"
#include "UIElement.h"

class UITextbox : public UIElement, public IUITextElement
{
public:
	UITextbox();
	UITextbox(float x, float y);
	UITextbox(float x, float y, float width, float height);
	UITextbox(float x, float y, float width, float height, std::string Text);
	UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName);
	UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize);
	UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color);
	UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled);
	UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled, float roundedCornerRadiusX, float roundedCornerRadiusY);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Gets thickness of the button
	float GetStroke() { return this->Stroke; }

	// Gets distance from border to the text
	float GetMargins() { return this->Margins; }

	// Returns whether rounded corners are enabled or not
	bool GetRoundedCorners() { return this->RoundedCorners; }

	// Gets rounded corner X radius
	float GetRoundCornersRadiusX() { return this->roundCornerRadiusX; }

	// Gets rounded corner Y radius
	float GetRoundCornersRadiusY() { return this->roundCornerRadiusY; }


	// Moves text cursor one index to the left
	void ShiftCursorLeft() { if (cursorIndex > 0) cursorIndex--; if (cursorIndex < visibleStartIndex) visibleStartIndex--; }

	// Moves text cursor one index to the right
	void ShiftCursorRight() 
	{
		if (cursorIndex < Text.size()) { cursorIndex++; }
	}

	// Tells whether or not to draw the cursor after the text
	void ShouldDrawCursor(bool state) { this->shouldDrawCursor = state; }

	// Adds text to current text
	void AddText(std::string text) { this->Text += text; }

	// Adds text at cursor index
	void AppendTextAtCursorIndex(std::string text)
	{
		Text.insert(cursorIndex, text);
	}

	// Removes number of characters from the end of the text
	void RemoveCharacter()
	{ 
		std::string::iterator it = Text.begin() + cursorIndex;
		Text.erase(it);
		if (cursorIndex == visibleStartIndex)
		{
			if (visibleStartIndex > 0)
			{
				visibleStartIndex--;
			}
		}
		ShiftCursorLeft();
	}

	// Don't mess with this if you don't know what you're doing
	void __stdcall IncrementVisibleStartIndex() { if (Text.size() - characterLimit > visibleStartIndex) visibleStartIndex++; }

	// Don't mess with this if you don't know what you're doing
	void __stdcall DecrementVisibleStartIndex() { if (visibleStartIndex > 0) this->visibleStartIndex--; }

	// Sets thickness of the button if not filled
	void SetStroke(float stroke) { this->Stroke = stroke; }

	// Sets distance from the border to the text
	void SetMargins(float margins) { this->Margins = margins; }

	// Enables/Disables rounded corners
	void SetRoundedCorners(bool state) { this->RoundedCorners = state; }

	// Sets x and y radii for rounded corners
	void SetRoundedCornersRadii(float radX, float radY) { this->roundCornerRadiusX = radX; this->roundCornerRadiusY = radY; }

	// Set enabled state of the element
	void SetEnabled(bool state)
	{
		enabled = state;
		if (state)
		{
			color.a = normalAlpha;
			textColor.a = normalAlpha;
		}
		else
		{
			color.a = 36;
			textColor.a = 36;
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
				this->SetTextColor(newTextColor);

				Sleep(sleepIntervals);
			}
			this->SetVisible(false);
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
				this->SetTextColor(newTextColor);

				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

private:
	void SetDefaultOptions();
	void SetupEventListeners();

	float Stroke = 1.0f;
	float Margins = 0.1f;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;

	/* Options Specific to UITextbox */
	bool shouldDrawCursor = false; // draw cursor only when typing (only when textbox is active)
	std::string previousText = Text;
	int visibleStartIndex = 0;
	int cursorIndex = 0;
	int characterLimit = 10;
	int __stdcall CalculateVisibleTextLimit(int fontSize);
	std::string __stdcall getOnlyVisibleText();
	void __stdcall adjustVisibleStartIndex();

	~UITextbox();
};

