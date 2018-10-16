#include "UIImage.h"

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath)
{
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
	this->width = GetImageWidth();
	this->height = GetImageHeight();
}

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos) : UIElement(xPos, yPos)
{
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
	this->width = GetImageWidth();
	this->height = GetImageHeight();
}

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos, float width, float height) : UIElement(xPos, yPos, width, height)
{
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
}

UIImage::~UIImage()
{
}

void UIImage::LoadImageFile(UIWindow* window, const char* imgFilepath)
{
	this->bitmapImage = window->GetGraphics()->mcLoadImage(imgFilepath);
}

void __stdcall UIImage::Draw(Graphics* graphics)
{
	graphics->DrawBitmap(bitmapImage, xPos, yPos, width, height, imageOpacity);
}
