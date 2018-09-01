#include "UIProgressBar.h"

UIProgressBar::UIProgressBar()
{
	this->xPos = 40;
	this->yPos = 60;
	this->width = 140;
	this->height = 20;
}

UIProgressBar::UIProgressBar(float xPos, float yPos) : UIElement(xPos, yPos)
{
	this->width = 140;
	this->height = 20;
}

UIProgressBar::UIProgressBar(float xPos, float yPos, float width, float height) : UIElement(xPos, yPos, width, height)
{
}

UIProgressBar::~UIProgressBar()
{
}

float __stdcall UIProgressBar::calculateProgressBarWidth()
{
	float percentage = Value / MaxValue;
	return width * percentage;
}

void __stdcall UIProgressBar::Draw(Graphics* graphics)
{
	if (!this->RoundedCorners)
	{
		// Drawing the area for the progress bar
		graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, 1.0f, true);

		// Drawing the progress bar
		graphics->DrawRectangle(xPos, yPos, calculateProgressBarWidth(), height,
			progressColor.r, progressColor.g, progressColor.b, progressColor.a, 1.0f, true);
	}
	else
	{
		// Drawing the area for the progress bar
		graphics->DrawRoundedRectangle(xPos, yPos, width, height, roundCornerRadiusX, roundCornerRadiusY,
			color.r, color.g, color.b, color.a, 1.0f, true);

		// Drawing the progress bar
		graphics->DrawRoundedRectangle(xPos, yPos, calculateProgressBarWidth(), height, roundCornerRadiusX, roundCornerRadiusY,
			progressColor.r, progressColor.g, progressColor.b, progressColor.a, 1.0f, true);
	}
}
