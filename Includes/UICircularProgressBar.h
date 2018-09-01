#pragma once

#include "UIElement.h"
#include "IUIProgressElement.h"

class UICircularProgressBar : public UIElement, public IUIProgressElement
{
public:
	UICircularProgressBar();
	UICircularProgressBar(float xPos, float yPos);
	UICircularProgressBar(float xPos, float yPos, float radius);
	UICircularProgressBar(float xPos, float yPos, float radius, float stroke);

	// Draws the element on the screen and has to be called between graphics.BeginDraw() and graphics.EndDraw()
	void __stdcall Draw(Graphics* graphics);

	// Returns thickness of the circle
	float GetStroke() { return this->Stroke; }

	// Returns radius of the circle in which progress arc will be shown
	float GetRadius() { return this->Radius; }

	// Returns color of the progress arc
	Color GetProgressColor() { return this->progressColor; }

	// Sets thickness of the circle
	void SetStroke(float stroke) { this->Stroke = stroke; }

	// Sets radius of the circle in which progress arc will be shown
	void SetRadius(float size) { this->Radius = size; UpdateArcEndPoint(); }

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

	// Sets x and y coordinates of the circular progress bar
	void SetPosition(float x, float y) { this->xPos = x; this->yPos = y; UpdateArcEndPoint(); }

	// Sets current value of the circular progress bar
	void SetValue(float value) { this->Value = value; UpdateArcEndPoint(); }

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

	~UICircularProgressBar();

private:
	void SetArcEndPoint(float x, float y) { this->arcEndPointX = x; this->arcEndPointY = y; }
	void UpdateArcEndPoint();
	D2D1_ARC_SIZE arcSize = D2D1_ARC_SIZE_SMALL;
	Color progressColor = Color::Cyan;
	float arcEndPointX = xPos;
	float arcEndPointY = yPos - Radius;
	float Stroke = 4.0f;
	float Radius = 20.0f;
};


