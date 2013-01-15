#include "GameFiles/Gui/GuiElement.h"
#include "../../MaloWLib/Safe.h"

GuiElement::GuiElement()
{
	this->zX = 0;
	this->zY = 0;
	this->zWidth = 0;
	this->zHeight = 0;
	this->zGuiImage = 0;
	this->zOpacity = 0.0f;
	this->zHidden = true;
	this->zTextureName = "Unknown";
}

GuiElement::GuiElement(float x, float y, float width, float height, std::string textureName)
{
	this->zX = x;
	this->zY = y;
	this->zGuiImage = 0;
	this->zHidden = true;
	this->zOpacity = 0.0f;
	this->zWidth = width;
	this->zHeight = height;
	this->zTextureName = textureName;
}

GuiElement::~GuiElement()
{
	SAFE_DELETE(this->zGuiImage);
}

bool GuiElement::AddToRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		this->zGuiImage = ge->CreateImage(Vector2(this->zX , this->zY), this->GetDimension(), this->zTextureName.c_str());
		this->ShowGui();
		return true;
	}
	return false;
}

bool GuiElement::RemoveFromRenderer(GraphicsEngine* ge)
{
	if (this->zGuiImage)
	{
		ge->DeleteImage(this->zGuiImage);
		this->zGuiImage = 0;
		this->zHidden = true;
		return true;
	}
	return false;
}

void GuiElement::HideGui()
{
	if (this->zGuiImage)
	{
		this->zHidden = true;
		this->zOpacity = 0.0f;
		this->zGuiImage->SetOpacity(this->zOpacity);
	}
}

void GuiElement::ShowGui()
{
	if (this->zGuiImage)
	{
		this->zHidden = false;
		this->zOpacity = 1.0f;
		this->zGuiImage->SetOpacity(this->zOpacity);
	}
}

void GuiElement::FadeOut(float value)
{
	if (this->zGuiImage)
	{
		if (this->zOpacity > 0.0f)
		{
			this->zOpacity -= value;
			this->zGuiImage->SetOpacity(this->zOpacity);
		}
		else
		{
			this->HideGui();
		}
	}
}
