#include "InventorySlotGui.h"
#include <Safe.h>

InventorySlotGui::InventorySlotGui()
{
	this->zSlotImage = NULL;
}

InventorySlotGui::InventorySlotGui(float x, float y, float width, float height, Gui_Item_Data gid)
	: GuiElement(x, y, width, height, "Media/InGameUI/SlotBG.png")
{
	this->zSlotImage = NULL;

	this->zStackText = NULL;

	this->zGid = gid;

	if(this->zGid.zFilePath == "Unknown")
	{
		this->zBlocked = false;
	}
	else
	{
		this->zBlocked = true;
	}

	this->zEquipped = false;
	this->zBlocker = false;
}

InventorySlotGui::~InventorySlotGui()
{
	if ( this->zSlotImage && GetGraphics()->IsRunning()) 
		GetGraphics()->DeleteImage(this->zSlotImage);
	else if( this->zSlotImage && !GetGraphics()->IsRunning())
		throw("Image Memory Leak!");

	if(this->zStackText && GetGraphics()->IsRunning())
		GetGraphics()->DeleteText(this->zStackText);
	else if( this->zStackText && !GetGraphics()->IsRunning())
		throw("Image Memory Leak!");
}

bool InventorySlotGui::AddToRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		GuiElement::AddToRenderer(ge); 
		this->SetStrata(310.0f);
		if(this->zGid.zCanStack && !this->zStackText)
		{
			this->zStackText = ge->CreateText(MaloW::convertNrToString((float)this->zGid.zStacks).c_str(), GetPosition(), 0.5f, "Media/Fonts/new");
			this->zStackText->SetStrata(250.0f);
		}

		if(!this->zSlotImage && this->zGid.zFilePath != "Unknown")
		{
			this->zSlotImage = ge->CreateImage(Vector2(this->zX, this->zY), this->GetDimension(), this->zGid.zFilePath.c_str());
			this->zSlotImage->SetStrata(300.0f);
		}
		this->ShowGui();
		return true;
	}
	return false;
}

bool InventorySlotGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	if(this->zStackText)
	{
		ge->DeleteText(this->zStackText);
		this->zStackText = 0;
	}
	if (this->zGuiImage)
	{
		
		ge->DeleteImage(this->zGuiImage);
		this->zGuiImage = 0;
		this->zHidden = true;
	}
	if(this->zSlotImage)
	{
		ge->DeleteImage(this->zSlotImage);
		this->zSlotImage = 0;
	}
	return true;
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

void InventorySlotGui::AddItemToSlot( Gui_Item_Data gid, bool invOpen, GraphicsEngine* ge )
{
	this->zGid = gid;
	if(invOpen)
	{
		if(this->zGid.zCanStack && !this->zStackText)
		{
			this->zStackText = ge->CreateText(MaloW::convertNrToString((float)this->zGid.zStacks).c_str(), GetPosition(), 0.5f, "Media/Fonts/new");
			this->zStackText->SetStrata(150.0f);
		}
		if(!this->zSlotImage && this->zGid.zFilePath != "Unknown")
		{
			this->zSlotImage = ge->CreateImage(Vector2(this->zX, this->zY), this->GetDimension(), this->zGid.zFilePath.c_str());
			this->zSlotImage->SetStrata(200.0f);
		}
	}
	this->zBlocked = true;
	this->zBlocker = false;
}

void InventorySlotGui::RemoveItemFromSlot( bool invOpen, GraphicsEngine* ge )
{
	this->zGid = Gui_Item_Data();
	if(invOpen)
	{
		if(this->zStackText)
		{
			ge->DeleteText(this->zStackText);
			this->zStackText = NULL;
		}
		if(this->zSlotImage)
		{
			ge->DeleteImage(this->zSlotImage);
			this->zSlotImage = NULL;
		}
	}
	this->zBlocked = false;
	this->zBlocker = false;
}

