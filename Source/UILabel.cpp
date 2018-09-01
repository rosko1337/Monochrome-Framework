#include "UILabel.h"

UILabel::UILabel()
{
	SetDefaultOptions();
}

UILabel::UILabel(float x, float y) : UIElement(x, y)
{
	SetDefaultOptions();
}

UILabel::UILabel(float x, float y, float width, float height) : UIElement(x, y, width, height)
{
	// no other options need to be set, UIElement's constructor does the job
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text) : UIElement(x, y, width, height)
{
	this->Text = Text;
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text, std::string FontName) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
}

UILabel::UILabel(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled, float roundedCornerRadiusX, float roundedCornerRadiusY)
	: UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
	this->roundCornerRadiusX = roundedCornerRadiusX;
	this->roundCornerRadiusY = roundedCornerRadiusY;
}

void UILabel::SetDefaultOptions()
{
	width = 100;
	height = 40;
	color = Color(140, 140, 140, 255);
}

void __stdcall UILabel::Draw(Graphics * graphics)
{
	if (!this->RoundedCorners)
	{
		graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, Stroke, Filled);
	}
	else
	{
		graphics->DrawRoundedRectangle(xPos, yPos, width, height, roundCornerRadiusX, roundCornerRadiusY,
			color.r, color.g, color.b, color.a, Stroke, Filled);
	}

	DWRITE_TEXT_ALIGNMENT textAllignment;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAllignment;
	MakeTextAllignment(this->TextAllignment, textAllignment, paragraphAllignment);

	graphics->drawText(Text, FontName, FontSize, fontStyle, fontWeight,
		xPos + Margins, yPos, width - Margins, height, textColor.r, textColor.g, textColor.b, textColor.a,
		textAllignment, paragraphAllignment);
}
