#include "GameFiles/Gui/CircularListGui.h"
#include "../../MaloWLib/Safe.h"

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
	SAFE_DELETE(this->zItemImage);
}

bool CircularListGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	if (ge)
	{
		//this->zItemImage = ge->CreateImage(Vector2(this->zItemX, this->zItemY), Vector2(this->zItemWidth, this->zItemHeight), this->zItemImageName.c_str());
		//this->ShowGui();
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

int CircularListGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();

	if(mousePressed)
	{
		if(mouseX < (this->zX + this->zWidth * 0.5f) && mouseY < (this->zY + this->zHeight * 0.5f))
			return 0;
		else if(mouseX > (this->zX + this->zWidth * 0.5f) && mouseY < (this->zY + this->zHeight * 0.5f))
			return 1;
		else if(mouseX < (this->zX + this->zWidth * 0.5f) && mouseY > (this->zY + this->zHeight * 0.5f))
			return 2;
		else if(mouseX > (this->zX + this->zWidth * 0.5f) && mouseY > (this->zY + this->zHeight * 0.5f))
			return 3;
	}

	return -1;
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