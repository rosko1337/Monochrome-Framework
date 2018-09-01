#pragma once

#include "EventListener.h"
#include "Keyboard.hpp"

class KeypressEventListener
{
public:
	// Keyboard callback function prototype
	typedef void(*keybd_callback_function)(UIElement*, const char*);

	KeypressEventListener(UIElement* sender, keybd_callback_function callbackFunc);

	void StartListener();
	void StopListener() { this->listenerShouldStop = true; }
	
	~KeypressEventListener();

private:
	bool listenerShouldStop = false;
	keybd_callback_function CallbackFunc;
	UIElement* sender;

	void DetectKeypress();
	bool isKeyPressed = false;
	const char* key = '\0';

	bool CheckKey(int keyCode, const char* result, const char* resultWithShiftPressed);
};

