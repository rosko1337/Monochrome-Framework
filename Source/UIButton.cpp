#include "UIButton.h"

void Mouse_HoverOn(UIElement* sender)
{
	UIButton* btn = static_cast<UIButton*>(sender);
	btn->SetHoverOffColor(btn->GetColor());
	btn->SetColor(Color(btn->GetHoverOnColor().r, btn->GetHoverOnColor().g, btn->GetHoverOnColor().b, btn->GetColor().a));
}

void Mouse_HoverOff(UIElement* sender)
{
	UIButton* btn = static_cast<UIButton*>(sender);
	btn->SetColor(btn->GetHoverOffColor());
}

UIButton::UIButton()
{
	SetDefaultOptions();
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y) : UIElement(x, y)
{
	SetDefaultOptions();
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height) : UIElement(x, y, width, height)
{
	// no other options need to be set, UIElement's constructor does the job
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text) : UIElement(x, y, width, height)
{
	this->Text = Text;
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text, std::string FontName) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
	SetInternalHoverEventListeners();
}

UIButton::UIButton(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled, float roundedCornerRadiusX, float roundedCornerRadiusY)
	: UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
	this->roundCornerRadiusX = roundedCornerRadiusX;
	this->roundCornerRadiusY = roundedCornerRadiusY;
	SetInternalHoverEventListeners();
}

void UIButton::SetDefaultOptions()
{
	width = 100;
	height = 40;
	color = Color(140, 140, 140, 255);
}

void __stdcall UIButton::Draw(Graphics* graphics)
{
	if (!this->RoundedCorners)
	{
		graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, Stroke, Filled); // button itself

		graphics->DrawRectangle(xPos - 0.2f, yPos - 0.2f, width + 0.2f, height + 0.2f,
			borderColor.r, borderColor.g, borderColor.b, borderColor.a, borderStroke, false); // border rectangle to indicate that its a button
	}
	else
	{
		graphics->DrawRoundedRectangle(xPos, yPos, width, height, roundCornerRadiusX, roundCornerRadiusY,
			color.r, color.g, color.b, color.a, Stroke, Filled); // button itself

		graphics->DrawRoundedRectangle(xPos - 0.2f, yPos - 0.2f, width + 0.2f, height + 0.2f, roundCornerRadiusX, roundCornerRadiusY,
			borderColor.r, borderColor.g, borderColor.b, borderColor.a, borderStroke, false); // border rectangle to indicate that its a button
	}

	DWRITE_TEXT_ALIGNMENT textAllignment;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAllignment;
	MakeTextAllignment(this->TextAllignment, textAllignment, paragraphAllignment);

	graphics->drawText(Text, FontName, FontSize, fontStyle, fontWeight,
		xPos + Margins, yPos, width - Margins, height, textColor.r, textColor.g, textColor.b, textColor.a,
		textAllignment, paragraphAllignment); // text inside a button
}

void UIButton::SetInternalHoverEventListeners()
{
	//new MouseHoverOnEventListener(this, Mouse_HoverOn);
	//new MouseHoverOffEventListener(this, Mouse_HoverOff);
	AddMouseHoverOnEvent(Mouse_HoverOn);
	AddMouseHoverOffEvent(Mouse_HoverOff);
}

void UIButton::EmergencySetHoverOffColor()
{
	this->color = hoverOffColor;
}
