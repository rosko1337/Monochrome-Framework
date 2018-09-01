#include "IUITextElement.h"

IUITextElement::IUITextElement(std::string text)
{
	this->Text = text;
}

IUITextElement::IUITextElement(std::string text, std::string fontName)
{
	this->Text = text;
	this->FontName = fontName;
}

IUITextElement::IUITextElement(std::string text, std::string fontName, int fontSize)
{
	this->Text = text;
	this->FontName = fontName;
	this->FontSize = fontSize;
}
