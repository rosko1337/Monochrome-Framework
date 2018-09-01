#pragma once

#include "UIElement.h"
#include "IUIProgressElement.h"

class UIProgressBar : public UIElement, public IUIProgressElement
{
public:
	UIProgressBar();
	UIProgressBar(float xPos, float yPos);
	UIProgressBar(float xPos, float yPos, float width, float height);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Returns whether rounded corners are enabled or not
	bool GetRoundedCorners() { return this->RoundedCorners; }

	// Gets rounded corner X radius
	float GetRoundCornersRadiusX() { return this->roundCornerRadiusX; }

	// Gets rounded corner Y radius
	float GetRoundCornersRadiusY() { return this->roundCornerRadiusY; }

	// Enables/Disables rounded corners
	void SetRoundedCorners(bool state) { this->RoundedCorners = state; }

	// Sets x and y radii for rounded corners
	void SetRoundedCornersRadii(float radX, float radY) { this->roundCornerRadiusX = radX; this->roundCornerRadiusY = radY; }

	// Returns color of the progress arc
	Color GetProgressColor() { return this->progressColor; }

	// Sets color of the progress arc
	void SetProgressColor(Color color) { this->progressColor = color; }

	// Set enabled state of the element
	void SetEnabled(bool state)
	{
		this->enabled = state;
		if (state)
		{
			color.a = normalAlpha;
			progressColor.a = normalAlpha;
		}
		else
		{
			color.a = 36;
			progressColor.a = 36;
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

				Color newProgressColor = GetProgressColor();
				newProgressColor.a = currentColor.a;
				this->SetProgressColor(newProgressColor);

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


				Color newProgressColor = GetProgressColor();
				newProgressColor.a = color.a;
				this->SetProgressColor(newProgressColor);

				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

	~UIProgressBar();

private:
	Color progressColor = Color::Cyan;
	bool RoundedCorners = true;
	float roundCornerRadiusX = 4, roundCornerRadiusY = 4;
	float __stdcall calculateProgressBarWidth();
};

