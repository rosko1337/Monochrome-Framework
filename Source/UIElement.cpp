#include "UIElement.h"
#include "MouseClickEventListener.h"
#include "MouseRightClickEventListener.h"
#include "MouseDownEventListener.h"
#include "MouseHoverOnEventListener.h"
#include "MouseHoverOffEventListener.h"

void UIElement::AddMouseDownEventListener(callback_function callbackFunc)
{
	if (this->MouseDownListener != nullptr) { this->MouseDownListener->StopListener(); }
	this->MouseDownListener = new MouseDownEventListener(this, callbackFunc);
}

void UIElement::AddMouseClickEventListener(callback_function callbackFunc)
{
	if (this->MouseClickListener != nullptr) { this->MouseClickListener->StopListener(); }
	this->MouseClickListener = new MouseClickEventListener(this, callbackFunc);
}

void UIElement::AddRightMouseClickEventListener(callback_function callbackFunc)
{
	if (this->RightMouseClickListener != nullptr) { this->RightMouseClickListener->StopListener(); }
	this->RightMouseClickListener = new MouseRightClickEventListener(this, callbackFunc);
}

void UIElement::AddHoverOnEventListener(callback_function callbackFunc)
{
	if (this->MouseHoverOnListener != nullptr) { this->MouseHoverOnListener->StopListener(); }
	this->MouseHoverOnListener = new MouseHoverOnEventListener(this, callbackFunc);
}

void UIElement::AddHoverOffEventListener(callback_function callbackFunc)
{
	if (this->MouseHoverOffListener != nullptr) { this->MouseHoverOffListener->StopListener(); }
	this->MouseHoverOffListener = new MouseHoverOffEventListener(this, callbackFunc);
}
