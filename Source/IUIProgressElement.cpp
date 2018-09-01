#include "IUIProgressElement.h"
#include "ValueChangedEventListener.h"

void IUIProgressElement::AddValueChangedEventListener(value_changed_callback_function callbackFunc)
{
	if (this->ValueChangedListener != nullptr) { this->ValueChangedListener->StopListener(); }
	this->ValueChangedListener = new ValueChangedEventListener(this, callbackFunc);
}
