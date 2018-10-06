#pragma once

#include "UIElement.h"
#include "IUITextElement.h"

class UICheckbox : public UIElement, public IUITextElement
{
public:
	UICheckbox();
	UICheckbox(float xPos, float yPos);
	UICheckbox(float xPos, float yPos, float size);
	UICheckbox(float xPos, float yPos, float size, std::string text);
	UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName);
	UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName, int fontSize);
	UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName, int fontSize, float margins);

	void __stdcall Draw(Graphics* graphics);

	// Returns size of the checkbox
	float GetSize() { return this->Size; }

	// Gets distance from border to the text
	float GetMargins() { return this->Margins; }

	// Returns whether checkbox is checked or not
	bool IsChecked() { return this->Checked; }

	// Returns whether rounded corners are enabled or not
	bool GetRoundedCorners() { return this->RoundedCorners; }

	// Gets rounded corner X radius
	float GetRoundCornersRadiusX() { return this->roundCornerRadiusX; }

	// Gets rounded corner Y radius
	float GetRoundCornersRadiusY() { return this->roundCornerRadiusY; }

	// Returns the size of the checkmark
	int GetCheckMarkFontSize() { return this->checkmarkFontSize; }

	// Returns color of the checkmark
	Color GetCheckmarkColor() { return this->checkmarkColor; }

	// Sets color of the checkmark
	void SetCheckmarkColor(Color color) { this->checkmarkColor = color; }

	// Sets the font size of the checkmark
	void SetCheckmarkFontSize(int fontSize) { this->checkmarkFontSize = fontSize; }

	// Sets distance from the border to the text
	void SetMargins(float margins) { this->Margins = margins; }

	// Sets size of the checkbox
	void SetSize(float size) { this->Size = size; this->width = size; this->height = size; }

	// Sets whether checkbox is checked or not
	void SetChecked(bool state) { this->Checked = state; }
	
	// Sets size of the checkbox
	void SetWidth(float size) { this->Size = size; this->width = size; this->height = size; }

	// Sets size of the checkbox
	void SetHeight(float size) { this->Size = size; this->width = size; this->height = size; }

	// Sets enabled state of the checkbox
	void SetEnabled(bool state)
	{
		this->enabled = state;
		if (state)
		{
			color.a = normalAlpha;
			textColor.a = normalAlpha;
			checkmarkColor.a = normalAlpha;
		}
		else
		{
			color.a = 36;
			textColor.a = 36;
			checkmarkColor.a = 36;
		}
	}

	// Enables/Disables rounded corners
	void SetRoundedCorners(bool state) { this->RoundedCorners = state; }

	// Sets x and y radii for rounded corners
	void SetRoundedCornersRadii(float radX, float radY) { this->roundCornerRadiusX = radX; this->roundCornerRadiusY = radY; }

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
				checkmarkColor.a = currentColor.a;
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
				checkmarkColor.a = color.a;
				this->SetTextColor(newTextColor);

				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

	// Calls the callback function when checkbox's "checked" state changes
	void AddStateChangedEventHandler(callback_function callbackFunc);

	~UICheckbox();

private:
	void StartEventListeners();

	float Size = 20.0f;
	float Margins = 4.0f;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;
	float checkmarkFontSize = FontSize;
	Color checkmarkColor = Color::Black;

	// Checked State
	bool Checked = false;
};
