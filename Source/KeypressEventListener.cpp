#include "KeypressEventListener.h"

KeypressEventListener::KeypressEventListener(UIElement* sender, keybd_callback_function callbackFunc)
{
	this->sender = sender;
	this->CallbackFunc = callbackFunc;
	std::thread running_thread(&KeypressEventListener::StartListener, this);
	running_thread.detach();
}

KeypressEventListener::~KeypressEventListener()
{
}

void KeypressEventListener::StartListener()
{
	while (!this->listenerShouldStop)
	{
		Sleep(2);
		if (sender == nullptr || sender->GetSourceWindow() == nullptr) continue;

		if (sender->IsVisible() && sender->IsEnabled() && sender->GetSourceWindow()->IsActive())
		{
			DetectKeypress();
			if (isKeyPressed)
			{
				CallbackFunc(sender, key);
			}
		}
	}
}

bool KeypressEventListener::CheckKey(int keyCode, const char* result, const char* resultWithShiftPressed)
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

void KeypressEventListener::DetectKeypress()
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
