#include "CheckBox.h"

CheckBox::CheckBox() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;
	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mEvent = NULL;

	this->mPressed = false;

	this->mName = "";

	this->mOn = false;
	this->mCheckedTextureName = "";
	this->mCheckedImage = NULL;
}
CheckBox::CheckBox(float x, float y, float z, string textureName, float width, float height, string CheckedTextureName, bool on, GUIEvent* tempEvent, string name) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;
	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mName = name;

	this->mPressed = false;

	this->mEvent = tempEvent;

	this->mOn = on;
	this->mCheckedTextureName = CheckedTextureName;
	if(this->mOn)
	{
		this->mCheckedImage = GetGraphics()->CreateImage(Vector2(x, y), this->GetDimension(), this->mCheckedTextureName.c_str());
	}
	else
	{
		this->mCheckedImage = NULL;
	}
}
CheckBox::~CheckBox()
{
	if(this->mEvent)
		delete this->mEvent;
}
bool CheckBox::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	if(this->mOn && this->mCheckedImage == NULL)
	{
		Vector2 temp = this->GetPositionD3D();
		this->mCheckedImage = GetGraphics()->CreateImage(Vector2(temp.x, temp.y), this->GetDimension(), this->mCheckedTextureName.c_str());
	}
	return false;
}
bool CheckBox::RemoveFromRenderer(GraphicsEngine* ge)
{
	Element::RemoveFromRenderer(ge);
	if(this->mCheckedImage != NULL)
	{
		ge->DeleteImage(this->mCheckedImage);
		this->mCheckedImage = NULL;
	}
	return true;
}

GUIEvent* CheckBox::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	if(!((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight))))
	{
		if(!this->mPressed && mousePressed)
		{
			this->mPressed = true;
		}
		else if(mPressed && !mousePressed)
		{
			this->mPressed = false;
			if(this->mOn == false)
			{
				this->mOn = true;
				Vector2 temp = this->GetPositionD3D();
				if(this->mCheckedImage == NULL)
					this->mCheckedImage = ge->CreateImage(Vector2(temp.x, temp.y), this->GetDimension(), this->mCheckedTextureName.c_str());

				ChangeOptionEvent* newTempEvent = (ChangeOptionEvent*)this->mEvent;
				newTempEvent->SetValue("true");

				return this->mEvent;
			}
			else if(this->mOn)
			{
				this->mOn = false;
				if(this->mCheckedImage != NULL)
				{
					ge->DeleteImage(this->mCheckedImage);
					this->mCheckedImage = NULL;
				}
				ChangeOptionEvent* newTempEvent = (ChangeOptionEvent*)this->mEvent;
				newTempEvent->SetValue("false");

				return this->mEvent;
			}
		}
	}
	return NULL;

}
void CheckBox::SetChecked(bool checked)
{ 
	if(checked == this->mOn)
		return;
	else
	{
		this->mOn = checked;
		if(this->mOn && this->mCheckedImage == NULL)
		{
			this->mCheckedImage = GetGraphics()->CreateImage(this->GetPositionD3D(), this->GetDimension(), this->mCheckedTextureName.c_str());
		}
		else if(!this->mOn && this->mCheckedImage != NULL)
		{
			GetGraphics()->DeleteImage(this->mCheckedImage);
			this->mCheckedImage = NULL;
		}
	}
}

void CheckBox::Resize( int oldWindowWidth, int oldWindowHeight, int windowWidth, int windowHeight )
{
	Element::Resize(oldWindowWidth, oldWindowHeight, windowWidth, windowHeight);

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float oldDx = ((float)oldWindowHeight * 4.0f) / 3.0f;
	float oldOffSet = (float)(oldWindowWidth - oldDx) / 2.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	this->mActiveX = offSet + ((this->mActiveX - oldOffSet) / oldDx) * dx;
	this->mActiveY = (this->mActiveY / oldWindowHeight) * windowHeight;


	this->mActiveWidth = (this->mActiveWidth / oldDx) * dx;
	this->mActiveHeight = (this->mActiveHeight / oldWindowHeight) * windowHeight;

	if(this->mCheckedImage)
	{
		this->mCheckedImage->SetPosition(Element::GetPositionD3D());
		this->mCheckedImage->SetDimensions(Element::GetDimension());
	}
}
