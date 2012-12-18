#include "GameFiles/Gui/InventoryGui.h"

InventoryGui::InventoryGui()
{

}

InventoryGui::InventoryGui( float x, float y, float width, float height, std::string textureName )
{

}

InventoryGui::~InventoryGui()
{

}

bool InventoryGui::AddToRenderer( GraphicsEngine* ge )
{
	GuiElement::AddToRenderer(ge);
	return true;
}

bool InventoryGui::RemoveFromRenderer( GraphicsEngine* ge )
{
	GuiElement::RemoveFromRenderer(ge);
	return true;
}

bool InventoryGui::CheckCollision( float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge )
{
	return true;
}

