#include "UISlider.h"
#include "Mouse.h"

UISlider::UISlider()
{
	this->xPos = 80;
	this->yPos = 80;
	this->width = 140;
	this->height = 4;
	this->knobRadius = 8;
	SetupEventHandlers();
}

UISlider::UISlider(float x, float y) : UIElement(x, y)
{
	this->width = 140;
	this->height = 4;
	this->knobRadius = 6;
	SetupEventHandlers();
}

UISlider::UISlider(float x, float y, float width) : UIElement(x, y)
{
	this->width = width;
	this->height = 4;
	this->knobRadius = 8;
	SetupEventHandlers();
}

UISlider::UISlider(float x, float y, float width, float height) : UIElement(x, y, width, height)
{
	this->knobRadius = 8;
	SetupEventHandlers();
}

UISlider::UISlider(float x, float y, float width, float height, float knobRadius) : UIElement(x, y, width, height)
{
	this->knobRadius = knobRadius;
	SetupEventHandlers();
}

UISlider::~UISlider()
{
}

void __stdcall UISlider::Draw(Graphics* graphics)
{
	// draw a bar
	graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, 2, true);

	// draw a slider-knob / handle
	graphics->DrawCircle(calculateKnobPositionX(), yPos + (height / 2), knobRadius, knobColor.r, knobColor.g, knobColor.b, knobColor.a, 1, true);
}

float UISlider::calculateKnobPositionX()
{
	float percentage = Value / MaxValue;
	return xPos + (width * percentage);
}

void UISlider::SetupEventHandlers()
{
	std::thread mouseMovedWhilePressedThread([this]
	{
		bool on = false;
		while (true)
		{
			Sleep(12);
			if (this == nullptr || this->GetSourceWindow() == nullptr) continue;

			if (visible && enabled && GetSourceWindow()->IsActive())
			{
				if (GetAsyncKeyState(VK_LBUTTON) == 0)
				{
					on = false;
				}

				// Get Mouse Position
				POINT pos;
				pos.x = Mouse::GetMousePosX(GetSourceWindow());
				pos.y = Mouse::GetMousePosY(GetSourceWindow());

				// If Mouse is Pressed
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (pos.x < calculateKnobPositionX() + knobRadius && pos.x > calculateKnobPositionX() - this->GetKnobRadius() &&
						pos.y < this->GetPosY() + this->GetKnobRadius() && pos.y > this->GetPosY() - this->GetKnobRadius())
					{
						on = true;
					}
				}

				// Make Changes to the slider's knob and set new values
				if (on)
				{
					// Maximum
					if (pos.x >= this->GetPosX() + this->GetWidth())
					{
						SetValue(GetMaxValue());
					}
					// Minimum
					else if (pos.x <= GetPosX())
					{
						SetValue(GetMinValue());
					}
					// In the Middle
					else
					{
						float oldValue = GetValue();
						float knobPosX = (float)pos.x;
						float distanceToKnob = knobPosX - GetPosX();
						float valueInPercent = distanceToKnob / GetWidth();

						int tickCount = (int)((GetMaxValue() - GetMinValue()) / GetInterval());
						float distanceToEachTick = GetWidth() / (float)tickCount;

						int ticksPast = (int)(distanceToKnob / distanceToEachTick);
						float newValue = (float)ticksPast * GetInterval();

						SetValue(newValue);
					}
				}
			}
		}
	});
	mouseMovedWhilePressedThread.detach();
}
