#include "GameFiles/Gui/InventoryGui.h"

InventoryGui::InventoryGui()
{
	zNrOfSlots = Vector2(7, 7);
}

InventoryGui::InventoryGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	zNrOfSlots = Vector2(7, 7);
}

InventoryGui::~InventoryGui()
{
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		if ((*it))
		{
			delete (*it);
			(*it) = NULL;
		}
	}
}

bool InventoryGui::AddItemToGui(std::string textureName, GraphicsEngine* ge)
{
	int size = this->zSlotGui.size();
	int posX = (int)(size % (int)zNrOfSlots.x);
	int posY = (int)(size / zNrOfSlots.x);
	if (posX * posY < (zNrOfSlots.x * zNrOfSlots.y))
	{
		float width = 40.0f;
		float height = 40.0f;
		float x = this->zX + width * posX + 15;
		float y = this->zY + height * posY + 15;
		InventorySlotGui* gui = new InventorySlotGui(x, y, width, height, textureName);
		this->zSlotGui.push_back(gui);

		return true;
	}
	

	return false;
}

bool InventoryGui::RemoveItemFromGui(const unsigned int position)
{
	this->zSlotGui.erase(this->zSlotGui.begin() + position);
	std::vector<InventorySlotGui*> temp;
	int pos = 0;
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		if ((*it))
		{
			int posX = (temp.size() % (int)zNrOfSlots.x);
			int posY = (int)(temp.size() / zNrOfSlots.x);
			float width = 50.0f;
			float height = 50.0f;
			float x = this->zX + width * posX + 15;
			float y = this->zY + height * posY + 15;
			(*it)->SetPosition(x, y);
			temp.push_back((*it));
		}
		pos++;
	}
	this->zSlotGui = temp;

	return true;
}

bool InventoryGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		(*x)->AddToRenderer(ge);
	}
	return true;
}

bool InventoryGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		(*x)->RemoveFromRenderer(ge);
	}
	return true;
}

int InventoryGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();
	int counter = 0;
	bool bCollision = false;
	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end() && !bCollision; x++)
		{
			if ((*x))
			{
				bCollision = (*x)->CheckCollision(mouseX, mouseY, mousePressed, ge);
				if (bCollision)
				{
					return counter;
				}
			}
			counter++;
		}
	}
	return -1;
}

void InventoryGui::HideGui()
{
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->HideGui();
		}
	}
	GuiElement::HideGui();
}

void InventoryGui::ShowGui()
{
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->ShowGui();
		}
	}
	GuiElement::ShowGui();
}

void InventoryGui::FadeOut(float value)
{
	GuiElement::FadeOut(value);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->FadeOut(value);
		}
	}
}

std::string InventoryGui::GetImageName( unsigned int position )
{
	std::string ret;
	this->zSlotGui[position]->GetTextureName(ret);
	return ret;
}
