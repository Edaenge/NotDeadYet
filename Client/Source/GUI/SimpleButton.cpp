#include "SimpleButton.h"
#include "Safe.h"

SimpleButton::SimpleButton() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;
	
	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = "";
	this->mTextureNameHovered = "";

	this->mHoveredImage = NULL;
	this->mPressedImage = NULL;

	this->mEvent = NULL;
}
SimpleButton::~SimpleButton()
{
	SAFE_DELETE(this->mEvent);
}
SimpleButton::SimpleButton(float x, float y, float z, string textureName, float width, float height, GUIEvent* tempEvent,
		string mTextureNamePressed, string mTextureNameHovered, float activeX, float activeY, float activeWidth, float activeHeight) 
	: Element(x, y, z, textureName, width, height)
{

	this->mActiveX = activeX;
	this->mActiveY = activeY;
	
	this->mActiveWidth = activeWidth;
	this->mActiveHeight = activeHeight;

	this->mPressed = false;
	this->mHovered = false;

	this->mTextureNamePressed = mTextureNamePressed;
	this->mTextureNameHovered = mTextureNameHovered;

	this->mHoveredImage = NULL;
	this->mPressedImage = NULL;

	this->mEvent = tempEvent;
}
GUIEvent* SimpleButton::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	/*If Outside*/
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX + this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY + this->mActiveHeight)))
	{
		this->mPressed = false;
		if(this->mPressedImage != NULL)
		{
			ge->DeleteImage(this->mPressedImage);
			this->mPressedImage = NULL;
		}
		this->mHovered = false;
		if(this->mHoveredImage != NULL)
		{
			ge->DeleteImage(this->mHoveredImage);
			this->mHoveredImage = NULL;
		}
		return NULL;
	}
	else
	{
		if(mousePressed)
		{
			if(!this->mPressed)
			{
				this->mPressed = true;
				if(this->mPressedImage == NULL)
				{
					if(this->mTextureNamePressed != "")
						this->mPressedImage = ge->CreateImage(this->GetPosition(), this->GetDimension(), this->mTextureNamePressed.c_str()); 
				}

				this->mHovered = false;
				if(this->mHoveredImage != NULL)
				{
					ge->DeleteImage(this->mHoveredImage);
					this->mHoveredImage = NULL;
				}
			}
		}
		else if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					if(this->mTextureNameHovered != "")
						this->mHoveredImage = ge->CreateImage(this->GetPosition(), this->GetDimension(), this->mTextureNameHovered.c_str());
				}

				this->mPressed = false;
				if(this->mPressedImage != NULL)
				{
					ge->DeleteImage(this->mPressedImage);
					this->mPressedImage = NULL;
				}
				return this->GetEvent();
			}
			else
			{
				this->mHovered = true;
				if(this->mHoveredImage == NULL)
				{
					if(this->mTextureNameHovered != "")
						this->mHoveredImage = ge->CreateImage(this->GetPosition(), this->GetDimension(), this->mTextureNameHovered.c_str());
				}
			}
		}
	}
	return NULL;
}
bool SimpleButton::RemoveFromRenderer(GraphicsEngine* ge)
{
	if(this->mHoveredImage != NULL)
	{
		ge->DeleteImage(this->mHoveredImage);
		this->mHoveredImage = NULL;
	}
	if(this->mPressedImage != NULL)
	{
		ge->DeleteImage(this->mPressedImage);
		this->mPressedImage = NULL;
	}
	Element::RemoveFromRenderer(ge);
	return true;
}

void SimpleButton::Resize( float oldWindowWidth, float oldWindowHeight, float windowWidth, float windowHeight )
{
	Element::Resize(oldWindowWidth, oldWindowHeight, windowWidth, windowHeight);

	float dx = (windowHeight * 4.0f) / 3.0f;
	float oldDx = (oldWindowHeight * 4.0f) / 3.0f;
	float oldOffSet = (oldWindowWidth - oldDx) / 2.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	this->mActiveX = offSet + ((this->mActiveX - oldOffSet) / oldDx) * dx;
	this->mActiveY = (this->mActiveY / oldWindowHeight) * windowHeight;

	this->mActiveWidth = (this->mActiveWidth / oldDx) * dx;
	this->mActiveHeight = (this->mActiveHeight / oldWindowHeight) * windowHeight;

	if(this->mHoveredImage)
	{
		this->mHoveredImage->SetPosition(this->GetPosition());
		this->mHoveredImage->SetDimensions(this->GetDimension());
	}
	if(this->mPressedImage)
	{
		this->mPressedImage->SetPosition(this->GetPosition());
		this->mPressedImage->SetDimensions(this->GetDimension());
	}
}
