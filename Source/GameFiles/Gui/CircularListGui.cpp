#include "GameFiles/Gui/CircularListGui.h"

CircularListGui::CircularListGui() 
	: GuiElement()
{
	this->zPressed = false;
	this->zHovered = false;
	this->zItemImage = NULL;
	this->zItemImageName = "";
}

CircularListGui::CircularListGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	this->zPressed = false;
	this->zHovered = false;
	this->zItemImageName = "Media/Inventory_v01.png";
	this->zItemX = (x + width) * 0.5f - 10.0f;
	this->zItemY = (y + height) * 0.5f - 10.0f;
	this->zItemWidth = 30.0f;
	this->zItemHeight = 30.0f;
	this->zItemImage = NULL;
}

CircularListGui::~CircularListGui()
{
	if (this->zItemImage)
	{
		delete this->zItemImage;
		this->zItemImage = NULL;
	}
}

bool CircularListGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	if (ge)
	{
		this->zItemImage = ge->CreateImage(Vector2(this->zItemX, this->zItemY), Vector2(this->zItemWidth, this->zItemHeight), this->zItemImageName.c_str());
		this->ShowGui();
		return true;
	}
	
	return true;
}

bool CircularListGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		GuiElement::RemoveFromRenderer(ge);
		if (this->zGuiImage)
		{
			ge->DeleteImage(this->zGuiImage);
			this->zGuiImage = 0;
		}
		return true;
	}
	

	return false;
}

bool CircularListGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();

	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		if (!this->zPressed && mousePressed)
		{
			this->zPressed = true;
			this->HideGui();
		}
	}
	return false;
}

void CircularListGui::HideGui()
{
	if (this->zItemImage)
	{
		this->zHidden = true;
		this->zOpacity = 0.0f;
		this->zItemImage->SetOpacity(this->zOpacity);
	}
	GuiElement::HideGui();
}

void CircularListGui::ShowGui()
{
	if (this->zItemImage)
	{
		this->zHidden = false;
		this->zOpacity = 1.0f;
		this->zItemImage->SetOpacity(this->zOpacity);
	}
	GuiElement::ShowGui();
}

void CircularListGui::FadeOut(float value)
{
	if (this->zItemImage)
	{
		if (this->zOpacity > 0.0f)
		{
			this->zOpacity -= value;
			this->zItemImage->SetOpacity(this->zOpacity);
		}
		else
		{
			this->HideGui();
		}
	}
	GuiElement::FadeOut(value);
}