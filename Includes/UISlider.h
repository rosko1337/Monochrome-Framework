#pragma once

#include "UIElement.h"
#include "IUIProgressElement.h"

class UISlider : public UIElement, public IUIProgressElement
{
public:
	UISlider();
	UISlider(float x, float y);
	UISlider(float x, float y, float width);
	UISlider(float x, float y, float width, float height);
	UISlider(float x, float y, float width, float height, float knobRadius);


	void __stdcall Draw(Graphics* graphics);

	// Returns radius of the knob
	float GetKnobRadius() { return knobRadius; }

	// Returns the color of the knob
	Color GetKnobColor() { return this->knobColor; }

	// Sets color of the knob
	void SetKnobColor(Color color) { this->knobColor = color; }
	
	// Sets radius of the know of the slider
	void SetKnobRadius(float radius) { this->knobRadius = radius; }

	// Sets enabled state of the slider
	void SetEnabled(bool state)
	{
		this->enabled = state;
		if (state)
		{
			color.a = normalAlpha;
			knobColor.a = normalAlpha;
		}
		else
		{
			color.a = 36;
			knobColor.a = 36;
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

				Color newKnobColor = GetKnobColor();
				newKnobColor.a = currentColor.a;
				this->SetKnobColor(newKnobColor);

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


				Color newKnobColor = GetKnobColor();
				newKnobColor.a = color.a;
				this->SetKnobColor(newKnobColor);

				this->normalAlpha = color.a;
			}
		});
		fadein_thread.detach();
	}

	~UISlider();

private:
	void SetupEventHandlers();
	Color knobColor = Color::White;
	float knobRadius = height;	// radius of the knob

	float calculateKnobPositionX();
};

