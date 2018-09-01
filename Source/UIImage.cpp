#include "UIImage.h"

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath)
{
	this->srcWindow = srcWindow;
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
	this->width = GetImageWidth();
	this->height = GetImageHeight();
}

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos) : UIElement(xPos, yPos)
{
	this->srcWindow = srcWindow;
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
	this->width = GetImageWidth();
	this->height = GetImageHeight();
}

UIImage::UIImage(UIWindow* srcWindow, const char* imgFilepath, float xPos, float yPos, float width, float height) : UIElement(xPos, yPos, width, height)
{
	this->srcWindow = srcWindow;
	this->bitmapImage = srcWindow->GetGraphics()->mcLoadImage(imgFilepath);
}

UIImage::~UIImage()
{
}

void __stdcall UIImage::Draw(Graphics* graphics)
{
	graphics->DrawBitmap(bitmapImage, xPos, yPos, width, height, imageOpacity);
}
