#include "GameFiles/Gui/InventorySlotGui.h"

InventorySlotGui::InventorySlotGui()
{
	this->zSlotImage = NULL;
}

InventorySlotGui::InventorySlotGui(float x, float y, float width, float height, std::string textureName)
	: GuiElement(x, y, width, height, textureName)
{
	this->zSlotImage = NULL;
}

InventorySlotGui::~InventorySlotGui()
{
	if (this->zSlotImage)
	{
		delete this->zSlotImage;
		this->zSlotImage = NULL;
	}
}

bool InventorySlotGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	return true;
}

bool InventorySlotGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);
	return true;
}

bool InventorySlotGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();
	bool bCollision = false;
	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		return true;
	}
	return false;
}

void InventorySlotGui::HideGui()
{
	if (this->zSlotImage)
	{
		this->zHidden = true;
		this->zOpacity = 0.0f;
		this->zSlotImage->SetOpacity(this->zOpacity);
	}
	GuiElement::HideGui();
}

void InventorySlotGui::ShowGui()
{
	if (this->zSlotImage)
	{
		this->zHidden = false;
		this->zOpacity = 1.0f;
		this->zSlotImage->SetOpacity(this->zOpacity);
	}
	GuiElement::ShowGui();
}

void InventorySlotGui::FadeOut(float value)
{
	if (this->zSlotImage)
	{
		if (this->zOpacity > 0.0f)
		{
			this->zOpacity -= value;
			this->zSlotImage->SetOpacity(this->zOpacity);
		}
		else
		{
			this->HideGui();
		}
	}
	
	GuiElement::FadeOut(value);
}