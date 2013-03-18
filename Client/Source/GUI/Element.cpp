#include "Element.h"

Element::Element()
{
	this->mX = 0;
	this->mY = 0;
	this->mZ = 0;

	this->mTextureName = "null";

	this->mWidth = 0;
	this->mHeight = 0;

	this->mImage = NULL;
}

Element::Element(float x, float y, float z, string textureName, float width, float height)
{
	this->mX = x;
	this->mY = y;
	this->mZ = z;
	
	this->mTextureName = textureName;

	this->mWidth = width;
	this->mHeight = height;
	
	this->mImage = NULL;
}
Element::~Element()
{

}
bool Element::AddToRenderer(GraphicsEngine* ge)
{
	if(!this->mImage && this->mTextureName != "")
	{
		this->mImage = ge->CreateImage(Vector2(this->mX, this->mY), Vector2(this->mWidth, this->mHeight), this->mTextureName.c_str());
		this->mImage->SetStrata(500.0f);
	}
	return true;
}
bool Element::RemoveFromRenderer(GraphicsEngine* ge)
{
	if(this->mImage)
		ge->DeleteImage(this->mImage);
	this->mImage = NULL;

	return true;
}

void Element::Resize(float oldWindowWidth, float oldWindowHeight, float windowWidth, float windowHeight)
{
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float oldDx = ((float)oldWindowHeight * 4.0f) / 3.0f;
	float oldOffSet = (float)(oldWindowWidth - oldDx) / 2.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	this->mX = offSet + ((this->mX - oldOffSet) / oldDx) * dx;
	this->mY = (this->mY / oldWindowHeight) * windowHeight;


	this->mWidth = (this->mWidth / oldDx) * dx;
	this->mHeight = (this->mHeight / oldWindowHeight) * windowHeight;

	if(this->mImage)
	{
		this->mImage->SetPosition(Vector2(this->mX, this->mY));
		this->mImage->SetDimensions(Vector2(this->mWidth, this->mHeight));
	}

}
