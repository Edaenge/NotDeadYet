#include "GameFiles/Gui/CircularListGui.h"

CircularListGui::CircularListGui() 
	: GuiElement()
{
	this->zPressed = false;
	this->zHovered = false;
}

CircularListGui::CircularListGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	this->zPressed = false;
	this->zHovered = false;
}

CircularListGui::~CircularListGui()
{

}

bool CircularListGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);

	return true;
}

bool CircularListGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);

	return true;
}

bool CircularListGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	float x;
	float y;
	this->GetPosition(x, y);
	Vector2 dimension = this->GetDimension();

	if (!((mouseX < x || mouseX > (x + dimension.x)) || (mouseY < y || mouseY > (y + dimension.y))))
	{
		if (!this->zPressed && mousePressed)
		{
			this->zPressed = true;
			this->HideGui();
		}
	}
	return false;
}