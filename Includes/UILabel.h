#pragma once

#include "UIElement.h"
#include "IUITextElement.h"

class UILabel : public UIElement, public IUITextElement
{
public:
	UILabel();
	UILabel(float x, float y);
	UILabel(float x, float y, float width, float height);
	UILabel(float x, float y, float width, float height, std::string Text);
	UILabel(float x, float y, float width, float height, std::string Text, std::string FontName);
	UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize);
	UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color);
	UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled);
	UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled, float roundedCornerRadiusX, float roundedCornerRadiusY);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Gets thickness of the button
	float GetStroke() { return this->Stroke; }

	// Gets distance from border to the text
	float GetMargins() { return this->Margins; }

	// Gets filled state
	bool GetFilled() { return this->Filled; }

	// Returns whether rounded corners are enabled or not
	bool GetRoundedCorners() { return this->RoundedCorners; }

	// Gets rounded corner X radius
	float GetRoundCornersRadiusX() { return this->roundCornerRadiusX; }

	// Gets rounded corner Y radius
	float GetRoundCornersRadiusY() { return this->roundCornerRadiusY; }


	// Sets thickness of the button if not filled
	void SetStroke(float stroke) { this->Stroke = stroke; }

	// Sets distance from the border to the text
	void SetMargins(float margins) { this->Margins = margins; }

	// Enables/Disables rounded corners
	void SetRoundedCorners(bool state) { this->RoundedCorners = state; }

	// Sets x and y radii for rounded corners
	void SetRoundedCornersRadii(float radX, float radY) { this->roundCornerRadiusX = radX; this->roundCornerRadiusY = radY; }

	// Should the area of the element be filled with color or not
	void SetFilled(bool state) { this->Filled = state; }

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
	float Stroke = 1.0f;
	float Margins = 0.1f;
	bool Filled = false;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;
};

