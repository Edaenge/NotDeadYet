#include "InventorySlotGui.h"
#include <Safe.h>

InventorySlotGui::InventorySlotGui()
{
	this->zSlotImage = NULL;
}

InventorySlotGui::InventorySlotGui(float x, float y, float width, float height, std::string textureName, int ID, int Type, int Stacks)
	: GuiElement(x, y, width, height, textureName)
{
	this->zSlotImage = NULL;
	this->zStackText = NULL;
	this->zID = ID;
	this->zType = Type;
	this->zStacks = Stacks;
}

InventorySlotGui::~InventorySlotGui()
{
	SAFE_DELETE(this->zSlotImage);
}

bool InventorySlotGui::AddToRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		GuiElement::AddToRenderer(ge);
		if(this->zStacks > 0 && !this->zStackText)
			this->zStackText = ge->CreateText(MaloW::convertNrToString((float)this->zStacks).c_str(), GetPosition(), 0.5f, "Media/Fonts/1");
		this->ShowGui();
		return true;
	}
	return false;
}

bool InventorySlotGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	if (this->zGuiImage)
	{
		if(this->zStackText)
		{
			ge->DeleteText(this->zStackText);
			this->zStackText = 0;
		}
		ge->DeleteImage(this->zGuiImage);
		this->zGuiImage = 0;
		this->zHidden = true;
		return true;
	}
	return false;
}

bool InventorySlotGui::CheckCollision(float mouseX, float mouseY)
{
	Vector2 dimension = this->GetDimension();
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

void InventorySlotGui::SetPosition( Vector2 pos )
{
	if(this->zStackText)
		this->zStackText->SetPosition(pos);
	GuiElement::SetPosition(pos);
}

void InventorySlotGui::SetDimension( Vector2 dim )
{
	GuiElement::SetDimension(dim);
}
