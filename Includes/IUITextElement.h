#pragma once

#include "UIElement.h"

class UITextbox;

// Dynamically casts IUIProgressElement to normal UIElement
#define ToUIElement(elem) dynamic_cast<UIElement*>(elem);

#define ToUITextbox(elem) static_cast<UITextbox*>(elem)


class IUITextElement
{
public:
	// Gets text on a button
	virtual std::string GetText() { return this->Text; }

	// Gets font name of the text
	virtual std::string GetFontName() { return this->FontName; }

	// Gets font size
	virtual int GetFontSize() { return this->FontSize; }

	// Get the style of the font
	virtual DWRITE_FONT_STYLE GetFontStyle() { return this->fontStyle; }

	// Get the weight of the font
	virtual DWRITE_FONT_WEIGHT GetFontWeight() { return this->fontWeight; }

	// Gets color of the text
	virtual Color GetTextColor() { return this->textColor; }


	// Sets text of the button
	virtual void SetText(std::string text) { this->Text = text; }

	// Sets the font of the text (e.x. TEXT_STYLE_ITALIC)
	virtual void SetFontName(std::string fontName) { this->FontName = fontName; }

	// Set the style of the font
	virtual void SetFontStyle(DWRITE_FONT_STYLE style) { this->fontStyle = style; }

	// Set the weight of the font (e.x. TEXT_WEIGHT_BOLD)
	virtual void SetFontWeight(DWRITE_FONT_WEIGHT weight) { this->fontWeight = weight; }

	// Sets TEXT_ALLIGNMENT to either center (default), left or right
	virtual void SetTextAllignment(int alligment) { this->TextAllignment = alligment; }

	// Sets size of the text
	virtual void SetFontSize(int size) { this->FontSize = size; }

	// Sets text color
	virtual void SetTextColor(Color color) { this->textColor = color; }

protected:
	std::string Text = std::string("Text");
	std::string FontName = std::string("Arial");
	int TextAllignment = TEXT_ALLIGNMENT_CENTER;
	DWRITE_FONT_STYLE fontStyle = FONT_STYLE_NORMAL;
	DWRITE_FONT_WEIGHT fontWeight = FONT_WEIGHT_NORMAL;
	int FontSize = 14;
	Color textColor = Color::Black;

	// Constructors
	IUITextElement() { /* Default Constructor */ }
	IUITextElement(std::string text);
	IUITextElement(std::string text, std::string fontName);
	IUITextElement(std::string text, std::string fontName, int fontSize);
};

