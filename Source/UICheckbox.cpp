#include "UICheckbox.h"
#include "MouseClickEventListener.h"

void Checkbox_OnClick(UIElement* sender)
{
	UICheckbox* cbx = static_cast<UICheckbox*>(sender);
	cbx->SetChecked(!cbx->IsChecked());
}

UICheckbox::UICheckbox()
{
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos) : UIElement(xPos, yPos)
{
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos, float size) : UIElement(xPos, yPos, size, size)
{
	this->Size = size;
	this->checkmarkFontSize = Size - 10;
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos, float size, std::string text) : UIElement(xPos, yPos, size, size), IUITextElement(text)
{
	this->Size = size;
	this->checkmarkFontSize = Size - 10;
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName) 
	: UIElement(xPos, yPos, size, size), IUITextElement(text, fontName)
{
	this->Size = size;
	this->checkmarkFontSize = Size - 10;
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName, int fontSize)
	: UIElement(xPos, yPos, size, size), IUITextElement(text, fontName, fontSize)
{
	this->Size = size;
	this->checkmarkFontSize = Size - 10;
	StartEventListeners();
}

UICheckbox::UICheckbox(float xPos, float yPos, float size, std::string text, std::string fontName, int fontSize, float margins)
	: UIElement(xPos, yPos, size, size), IUITextElement(text, fontName, fontSize)
{
	this->Size = size;
	this->Margins = margins;
	this->checkmarkFontSize = Size - 10;
	StartEventListeners();
}

UICheckbox::~UICheckbox()
{
}

void UICheckbox::StartEventListeners()
{
	new MouseClickEventListener(this, Checkbox_OnClick);
}

void __stdcall UICheckbox::Draw(Graphics* graphics)
{
	// Drawing the checkbox area
	if (!this->RoundedCorners)
	{
		graphics->DrawRectangle(xPos, yPos, Size, Size, color.r, color.g, color.b, color.a, 0, true);
	}
	else
	{
		graphics->DrawRoundedRectangle(xPos, yPos, Size, Size, roundCornerRadiusX, roundCornerRadiusY,
			color.r, color.g, color.b, color.a, 0, true);
	}

	// If checkbox is checked, draw the checkmark
	if (Checked)
	{
		graphics->drawText(L"✔", "Verdana", checkmarkFontSize, FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL, xPos + 0.2f, yPos + 0.2f, Size - 0.2f, Size - 0.2f, 0, 0, 0, 255,
			DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	// Drawing the text after the checkbox
	graphics->drawText(Text, FontName, FontSize, fontStyle, fontWeight, xPos + Size + Margins + 1, yPos, Text.size() * FontSize, Size,
		textColor.r, textColor.g, textColor.b, textColor.a, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void UICheckbox::AddStateChangedEventHandler(callback_function callbackFunc)
{
	std::thread callbackThread([this, callbackFunc]
	{
		bool previousCheckedState = Checked;
		while (true)
		{
			Sleep(14);
			if (visible && enabled)
			{
				bool currentCheckedState = Checked;
				if (currentCheckedState != previousCheckedState)
				{
					callbackFunc(this);
				}
				previousCheckedState = currentCheckedState;
			}
		}
	});
	callbackThread.detach();
}
