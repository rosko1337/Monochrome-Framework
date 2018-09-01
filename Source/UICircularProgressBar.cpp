#include "UICircularProgressBar.h"

UICircularProgressBar::UICircularProgressBar()
{
}

UICircularProgressBar::UICircularProgressBar(float xPos, float yPos) : UIElement(xPos, yPos)
{
	this->arcEndPointX = xPos;
	this->arcEndPointY = yPos - Radius;
}

UICircularProgressBar::UICircularProgressBar(float xPos, float yPos, float radius) : UIElement(xPos, yPos)
{
	this->Radius = radius;
	this->arcEndPointX = xPos;
	this->arcEndPointY = yPos - Radius;
}

UICircularProgressBar::UICircularProgressBar(float xPos, float yPos, float radius, float stroke) : UIElement(xPos, yPos)
{
	this->Radius = radius;
	this->Stroke = stroke;
	this->arcEndPointX = xPos;
	this->arcEndPointY = yPos - Radius;
}

UICircularProgressBar::~UICircularProgressBar()
{
}

void __stdcall UICircularProgressBar::Draw(Graphics* graphics)
{
	// Drawing the circle
	graphics->DrawCircle(xPos, yPos, Radius, color.r, color.g, color.b, color.a, Stroke, false);

	// Drawing the progress arc
	if (Value > MinValue)
	{
		graphics->DrawArc(xPos, yPos - Radius, arcEndPointX, arcEndPointY, Radius,
			D2D1_SWEEP_DIRECTION_CLOCKWISE, arcSize, progressColor.r, progressColor.g, progressColor.b, progressColor.a, Stroke);
	}
}

void UICircularProgressBar::UpdateArcEndPoint()
{
	if (this->Value > this->MaxValue)
	{
		this->Value = this->MaxValue;
		return;
	}
	else if (this->Value < this->MinValue)
	{
		this->Value = this->MinValue;
		return;
	}

	float valueRange = MaxValue - MinValue;
	float currentPercent = (Value - MinValue) / valueRange;
	if (currentPercent < 0.5f)
	{
		arcSize = D2D1_ARC_SIZE_SMALL;
	}
	else
	{
		arcSize = D2D1_ARC_SIZE_LARGE;
	}

	float angle = 2 * 3.14 * currentPercent;

	float xCoord = xPos + sinf(angle) * Radius;
	float yCoord = yPos - cosf(angle) * Radius;

	SetArcEndPoint(xCoord, yCoord);
}
