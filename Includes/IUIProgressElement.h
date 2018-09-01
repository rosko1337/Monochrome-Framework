#pragma once

class ValueChangedEventListener;
class UIElement;

class UISlider;
class UICircularProgressBar;
class UIProgressBar;

#define ToUISlider(elem) static_cast<UISlider*>(elem)
#define ToUICircularProgressBar(elem) static_cast<UICircularProgressBar*>(elem)
#define ToUIProgressBar(elem) static_cast<UIProgressBar*>(elem)

// Dynamically casts IUIProgressElement to normal UIElement
#define ToUIElement(elem) dynamic_cast<UIElement*>(elem);

class IUIProgressElement
{
public:
	// Returns current value
	virtual float GetValue() { return this->Value; }

	// Returns minimum value
	virtual float GetMinValue() { return this->MinValue; }

	// Returns maximum value
	virtual float GetMaxValue() { return this->MaxValue; }

	// Returns interval between values
	virtual float GetInterval() { return this->Interval; }

	// Sets current value
	virtual void SetValue(float value) { this->Value = value; }

	// Sets minimum value
	virtual void SetMinValue(float value) { this->MinValue = value; }

	// Sets maximum value
	virtual void SetMaxValue(float value) { this->MaxValue = value; }

	// Sets the interval between values
	virtual void SetInterval(float value) { this->Interval = value; }

	// Callback function prototype
	typedef void(*value_changed_callback_function)(IUIProgressElement*);

	// Calls the callback function when the value changes
	virtual void AddValueChangedEventListener(value_changed_callback_function callbackFunc);

protected:
	float Value = 0.0f;
	float Interval = 1.0f;
	float MinValue = 0.0f, MaxValue = 100.0f;

	// Each element is allowed to have only one listener of a type, therefore tracking is needed
	ValueChangedEventListener* ValueChangedListener;
};

