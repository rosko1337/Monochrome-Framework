#pragma once

#include "UIElement.h"

class UIImage : public UIElement
{
public:
	UIImage(UIWindow* srcWindow, const char* imgFilepath);
	UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos);
	UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos, float width, float height);

	// Loads a new image file
	void LoadImageFile(UIWindow* window, const char* imgFilepath);

	// Draws the image on the screen
	void __stdcall Draw(Graphics* graphics);
	
	// Returns alpha value of the image bitmap
	float GetImageOpacity() { return this->imageOpacity; }

	// Returns width of the original image
	float GetImageWidth() { return bitmapImage->GetSize().width; }

	// Returns height of the original image
	float GetImageHeight() { return bitmapImage->GetSize().height; }

	// Sets alpha channel value of the image
	void SetImageOpacity(float opacity) { this->imageOpacity = opacity; }

	~UIImage();
private:
	float imageOpacity = 1.0f;
	ID2D1Bitmap* bitmapImage;
};

