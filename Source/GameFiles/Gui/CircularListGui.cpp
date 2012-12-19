#include "GameFiles/Gui/CircularListGui.h"

CircularListGui::CircularListGui() 
	: GuiElement()
{
	this->zPressed = false;
	this->zHovered = false;
	this->zItemImageName = "";
}

CircularListGui::CircularListGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	this->zPressed = false;
	this->zHovered = false;
	this->zItemImageName = "";
	this->zItemX = (x + width) * 0.5f - 20.0f;
	this->zItemY = (y + height) * 0.5f - 20.0f;
	this->zItemWidth = 60.0f;
	this->zItemHeight = 60.0f;
}

CircularListGui::~CircularListGui()
{

}

bool CircularListGui::AddToRenderer(GraphicsEngine* ge, std::string itemTextureName)
{
	GuiElement::AddToRenderer(ge);

	this->zItemImage = ge->CreateImage(Vector2(this->zItemX, this->zItemY), Vector2(this->zItemWidth, this->zItemHeight), itemTextureName.c_str());
	
	return true;
}

bool CircularListGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);
	ge->DeleteImage(this->zItemImage);
	return true;
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