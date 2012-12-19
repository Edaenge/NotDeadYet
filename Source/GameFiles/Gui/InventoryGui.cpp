#include "GameFiles/Gui/InventoryGui.h"

InventoryGui::InventoryGui()
{
	zNrOfSlots = Vector2(7, 7);
}

InventoryGui::InventoryGui( float x, float y, float width, float height, std::string textureName )
{

}

InventoryGui::~InventoryGui()
{
	for (auto it = this->zSlotImage.begin(); it < this->zSlotImage.end(); it++)
	{
		if ((*it))
		{
			delete (*it);
			(*it) = NULL;
		}
	}
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
	Vector2 dimension = this->GetDimension();

	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		float x = this->zX;
		float y = this->zY;
		float slotWidth = 100.0f;
		float slotHeight = 100.0f;
		for (unsigned int n = 0; n < zNrOfSlots.y; n++)
		{
			for (unsigned int i = 0; i < zNrOfSlots.x; i++)
			{
				x += i * 120;
				y += n * 120;

				if ((mouseX > x && mouseX < (x + slotWidth)) && (mouseY > y && mouseY < (y + slotHeight)))
				{

				}
			}
		}
		
	}
	return true;
}

