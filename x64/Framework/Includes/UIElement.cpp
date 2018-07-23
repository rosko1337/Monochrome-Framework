#include "UIElement.h"
#include "GlobalWhileMouseDownEventHandler.h"
#include "MouseClickEventHandler.h"
#include "GlobalRightMouseClickEventHandler.h"
#include "GlobalHoverOnEventHandler.h"
#include "GlobalHoverOffEventHandler.h"

void UIElement::AddWhileMouseDownEventHandler(uiElement_global_callback_function callbackFunc)
{
	new GlobalWhileMouseDownEventHandler(this, callbackFunc);
}

void UIElement::AddMouseClickEventHandler(uiElement_global_callback_function callbackFunc)
{
	new MouseClickEventHandler(this, callbackFunc);
}

void UIElement::AddHoverOnEventHandler(uiElement_global_callback_function callbackFunc)
{
	new GlobalHoverOnEventHandler(this, callbackFunc);
}

void UIElement::AddHoverOffEventHandler(uiElement_global_callback_function callbackFunc)
{
	new GlobalHoverOffEventHandler(this, callbackFunc);
}

void UIElement::AddRightMouseClickEventHandler(uiElement_global_callback_function callbackFunc)
{
	new GlobalRightMouseClickEventHandler(this, callbackFunc);
}
