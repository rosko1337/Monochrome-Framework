#include "UITextbox.h"
#include "MouseClickAwayEventListener.h"
#include "Keyboard.hpp"
#include "Mouse.h"
#include <iostream>

void TextBox_ClickAway(UIElement* sender)
{
	UITextbox* textbox = static_cast<UITextbox*>(sender);
	textbox->SetActive(false);
	textbox->ShouldDrawCursor(false);
}

void TextBox_OnClick(UIElement* sender)
{
	UITextbox* textbox = static_cast<UITextbox*>(sender);
	textbox->SetActive(true);
	textbox->ShouldDrawCursor(true);
}

void TextBox_HoverOn(UIElement* sender)
{
	Mouse::SetNewCursor(Mouse::Text);
}

void TextBox_HoverOff(UIElement* sender)
{
	Mouse::SetNewCursor(Mouse::Default);
}

UITextbox::UITextbox()
{
	SetDefaultOptions();
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
}

UITextbox::UITextbox(float x, float y) : UIElement(x, y)
{
	SetDefaultOptions();
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height) : UIElement(x, y, width, height)
{
	// no other options need to be set, UIElement's constructor does the job
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text) : UIElement(x, y, width, height)
{
	this->Text = Text;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize) : UIElement(x, y, width, height)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled) : UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

UITextbox::UITextbox(float x, float y, float width, float height, std::string Text, std::string FontName, int FontSize, Color color, bool roundedCornersEnabled, float roundedCornerRadiusX, float roundedCornerRadiusY)
	: UIElement(x, y, width, height, color)
{
	this->Text = Text;
	this->FontName = FontName;
	this->FontSize = FontSize;
	this->RoundedCorners = roundedCornersEnabled;
	this->roundCornerRadiusX = roundedCornerRadiusX;
	this->roundCornerRadiusY = roundedCornerRadiusY;
	SetupEventListeners();
	TextAllignment = TEXT_ALLIGNMENT_LEFT;
	cursorIndex = Text.size();
}

void UITextbox::SetDefaultOptions()
{
	width = 100;
	height = 40;
	color = Color::White;
}

void UITextbox::SetupEventListeners()
{
	AddLeftMouseClickEvent(TextBox_OnClick);			// when user clicks on the textbox
	AddMouseHoverOnEvent(TextBox_HoverOn);				// changes cursor when mouse hovers over the textbox
	AddMouseHoverOffEvent(TextBox_HoverOff);			// changes cursor when mouse hovers off the textbox
	new MouseClickAwayEventListener(this, TextBox_ClickAway);	// checks if the mouse clicked outside of textbox to disable editing text and cursor showing up
	std::thread keybd_listener(&UITextbox::KeyboardListener, this); // detects key presses
	keybd_listener.detach();
}

UITextbox::~UITextbox()
{
}

void __stdcall UITextbox::Draw(Graphics* graphics)
{
	// Drawing the area similar to a label to display text in
	if (!this->RoundedCorners)
	{
		graphics->DrawRectangle(xPos, yPos, width, height, color.r, color.g, color.b, color.a, Stroke, true);
	}
	else
	{
		graphics->DrawRoundedRectangle(xPos, yPos, width, height, roundCornerRadiusX, roundCornerRadiusY,
			color.r, color.g, color.b, color.a, Stroke, true);
	}

	// Getting Text Allignment Parameters
	DWRITE_TEXT_ALIGNMENT textAllignment;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAllignment;
	MakeTextAllignment(this->TextAllignment, textAllignment, paragraphAllignment);

	characterLimit = CalculateVisibleTextLimit(FontSize); // getting visible character limit in the textbox
	adjustVisibleStartIndex(); // scrolling text if necessary
	std::string visibleText = getOnlyVisibleText(); // getting the part of the text that should be visible in the textbox

	// Drawing text
	graphics->drawText(visibleText, FontName, FontSize, fontStyle, fontWeight,
		xPos + Margins + 4, yPos, width - Margins, height, textColor.r, textColor.g, textColor.b, textColor.a,
		textAllignment, paragraphAllignment);
	
	// Drawing cursor
	if (shouldDrawCursor)
	{
		int visibleCursorIndex = cursorIndex - visibleStartIndex;

		// Text and Line Metrics
		float TextWidthInPixels;
		float maxWidth = width - Margins - 6;

		// getting width of the cut-off text
		graphics->GetWidthOfText(Text.substr(visibleStartIndex, visibleCursorIndex + 1), FontName, FontSize, fontStyle, fontWeight,
			textAllignment, width - Margins - 6, height, &TextWidthInPixels); // getting the new metrics

		float TextStartingPosX = (xPos + Margins + 4);
		graphics->drawText(L"|", "Arial", FontSize + 4, fontStyle, FONT_WEIGHT_NORMAL, TextStartingPosX + TextWidthInPixels, yPos - 2, width - Margins, height,
			textColor.r, textColor.g, textColor.b, textColor.a, textAllignment, paragraphAllignment);
	}

	previousText = Text; // updating previous text to current text
}

int __stdcall UITextbox::CalculateVisibleTextLimit(int fontSize)  // NEEDS WORK ASAP OR ELSE TEXTBOX IS GONNA BE BROKEN!!!a
{
	float cw = (float)fontSize * 1.3636f; // Width of 1 character
	int result = (int)((width / cw) - 1);
	return (result * 3 - 2);
}

std::string __stdcall UITextbox::getOnlyVisibleText()
{
	return Text.substr(visibleStartIndex, characterLimit);
}

void __stdcall UITextbox::adjustVisibleStartIndex()
{
	if (Text.size() < characterLimit) { visibleStartIndex = 0; return; } // all text fits in, no need to change anything

	if (Text.size() > previousText.size())
	{
		// text added
		if (Text.size() - characterLimit >= visibleStartIndex) visibleStartIndex++;
		cursorIndex++;

		// extra right shift to fix the bug
		if (cursorIndex >= Text.size() - 1)
		{
			IncrementVisibleStartIndex();
			IncrementVisibleStartIndex();
		}
	}
}

void UITextbox::AddTextChangedEventListener(text_changed_callback_function callbackFunc)
{
	this->TextChangedCallbackFunction = callbackFunc;
}

void UITextbox::KeyboardListener()
{
	while (true)
	{
		Sleep(2);
		if (this == nullptr || this->GetSourceWindow() == nullptr || !this->typingInProgress) continue;

		if (this->IsVisible() && this->IsEnabled() && this->GetSourceWindow()->IsActive())
		{
			DetectKeypress();
			if (isKeyPressed)
			{
				GetPressedKey(key);
			}
		}
	}
}

bool UITextbox::CheckKey(int keyCode, const char* result, const char* resultWithShiftPressed)
{
	if (Keyboard::IsKeyPressed(keyCode))
	{
		key = result;
		if (Keyboard::IsKeyDown(MC_LSHIFT) || Keyboard::IsKeyDown(MC_RSHIFT) || (GetKeyState(MC_CAPSLOCK) & 0x0001) != 0)
		{
			key = resultWithShiftPressed;
		}
		isKeyPressed = true;
		return true;
	}
	return false;
}

void UITextbox::DetectKeypress()
{
	isKeyPressed = false;

	// Action Keys
	if (CheckKey(MC_SPACE, " ", " ")) { return; }
	if (CheckKey(MC_BACKSPACE, "[DELETE]", "[DELETE]")) { return; }
	if (CheckKey(MC_ENTER, "[ENTER]", "[ENTER]")) { return; }
	if (CheckKey(MC_TAB, "    ", "    ")) { return; }
	if (CheckKey(MC_UP, "[UP]", "[UP]")) { return; }
	if (CheckKey(MC_DOWN, "[DOWN]", "[DOWN]")) { return; }
	if (CheckKey(MC_LEFT, "[LEFT]", "[LEFT]")) { return; }
	if (CheckKey(MC_RIGHT, "[RIGHT]", "[RIGHT]")) { return; }

	// Special Keys
	if (CheckKey(MC_BACKTICK, "`", "~")) { return; }
	if (CheckKey(MC_MINUS, "-", "_")) { return; }
	if (CheckKey(MC_EQUALS, "=", "+")) { return; }
	if (CheckKey(MC_SQUARE_BRACKET_RIGHT, "[", "{")) { return; }
	if (CheckKey(MC_SQUARE_BRACKET_LEFT, "]", "}")) { return; }
	if (CheckKey(MC_BACKSLASH, "\\", "|")) { return; }
	if (CheckKey(MC_SEMICOLON, ";", ":")) { return; }
	if (CheckKey(MC_TICK, "'", "\"")) { return; }
	if (CheckKey(MC_COMMA, ",", "<")) { return; }
	if (CheckKey(MC_PERIOD, ".", ">")) { return; }
	if (CheckKey(MC_SLASH, "/", "?")) { return; }

	// Numbers
	if (CheckKey(MC_KEY0, "0", ")")) { return; }
	if (CheckKey(MC_KEY1, "1", "!")) { return; }
	if (CheckKey(MC_KEY2, "2", "@")) { return; }
	if (CheckKey(MC_KEY3, "3", "#")) { return; }
	if (CheckKey(MC_KEY4, "4", "$")) { return; }
	if (CheckKey(MC_KEY5, "5", "%")) { return; }
	if (CheckKey(MC_KEY6, "6", "^")) { return; }
	if (CheckKey(MC_KEY7, "7", "&")) { return; }
	if (CheckKey(MC_KEY8, "8", "*")) { return; }
	if (CheckKey(MC_KEY9, "9", "(")) { return; }

	// Letters
	if (CheckKey(MC_KEY_A, "a", "A")) { return; }
	if (CheckKey(MC_KEY_B, "b", "B")) { return; }
	if (CheckKey(MC_KEY_C, "c", "C")) { return; }
	if (CheckKey(MC_KEY_D, "d", "D")) { return; }
	if (CheckKey(MC_KEY_E, "e", "E")) { return; }
	if (CheckKey(MC_KEY_F, "f", "F")) { return; }
	if (CheckKey(MC_KEY_G, "g", "G")) { return; }
	if (CheckKey(MC_KEY_H, "h", "H")) { return; }
	if (CheckKey(MC_KEY_I, "i", "I")) { return; }
	if (CheckKey(MC_KEY_J, "j", "J")) { return; }
	if (CheckKey(MC_KEY_K, "k", "K")) { return; }
	if (CheckKey(MC_KEY_L, "l", "L")) { return; }
	if (CheckKey(MC_KEY_M, "m", "M")) { return; }
	if (CheckKey(MC_KEY_N, "n", "N")) { return; }
	if (CheckKey(MC_KEY_O, "o", "O")) { return; }
	if (CheckKey(MC_KEY_P, "p", "P")) { return; }
	if (CheckKey(MC_KEY_Q, "q", "Q")) { return; }
	if (CheckKey(MC_KEY_R, "r", "R")) { return; }
	if (CheckKey(MC_KEY_S, "s", "S")) { return; }
	if (CheckKey(MC_KEY_T, "t", "T")) { return; }
	if (CheckKey(MC_KEY_U, "u", "U")) { return; }
	if (CheckKey(MC_KEY_V, "v", "V")) { return; }
	if (CheckKey(MC_KEY_W, "w", "W")) { return; }
	if (CheckKey(MC_KEY_X, "x", "X")) { return; }
	if (CheckKey(MC_KEY_Y, "y", "Y")) { return; }
	if (CheckKey(MC_KEY_Z, "z", "Z")) { return; }
}

void UITextbox::GetPressedKey(const char* keyPressed)
{
	UITextbox* textbox = static_cast<UITextbox*>(this);
	if (textbox->IsActive())
	{
		std::string key = std::string(keyPressed);
		// if textbox has TextChangedEventListener, then execute the callback function
		if (textbox->GetTextChangedCallbackFunction() != nullptr)
		{
			textbox->GetTextChangedCallbackFunction()(this, key);
		}

		// parsing out unnecessary keys
		if (key._Equal("[UP]") || key._Equal("[DOWN]") || key._Equal("[ENTER]")) return;

		// parsing character deletion
		if (key._Equal("[DELETE]"))
		{
			textbox->RemoveCharacter();
			return;
		}

		// parsing arrow keys to move text cursor
		if (key._Equal("[LEFT]"))
		{
			textbox->ShiftCursorLeft();
			return;
		}
		if (key._Equal("[RIGHT]"))
		{
			textbox->ShiftCursorRight();
			return;
		}

		// parsing tab key
		if (key._Equal("    "))
		{
			textbox->AppendTextAtCursorIndex(key);

			// for every space, increment visible start index
			textbox->IncrementVisibleStartIndex();
			textbox->IncrementVisibleStartIndex();
			textbox->IncrementVisibleStartIndex();

			// for every space, increment cursor index
			textbox->ShiftCursorRight();
			textbox->ShiftCursorRight();
			textbox->ShiftCursorRight();

			return;
		}

		textbox->ShiftCursorRight(); // one extra possible shift just in case (error handling included in the function)

									 // Adding the text to the textbox
		textbox->AppendTextAtCursorIndex(key);
	}
}
