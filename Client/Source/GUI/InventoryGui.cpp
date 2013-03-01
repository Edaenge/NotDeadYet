#include "InventoryGui.h"
#include <Safe.h>


#define SLOTIMAGEWIDTH 50.0f
#define SLOTIMAGEHEIGHT 50.0f
#define PADDING 12.0f
#define XOFFSETINV 32.0f
#define YOFFSETINV 206.0f

#define YOFFSETEQ 62.0f
static const int EQXPOS[] = {94, 280, 342};

InventoryGui::InventoryGui()
{
	zPressed = false;
}

InventoryGui::InventoryGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	float windowHeight = (float)(GetGraphics()->GetEngineParameters().WindowHeight);
	float dx = ((float)windowHeight * 4.0f) / 3.0f;

	float startOffsetX = (XOFFSETINV / 1024.0f) * dx;
	float startOffsetY = (YOFFSETINV / 768.0f) * GetGraphics()->GetEngineParameters().WindowHeight;
	this->zSlotImageWidth = (SLOTIMAGEWIDTH / 1024.0f) * dx;
	this->zSlotImageHeight = (SLOTIMAGEHEIGHT / 768.0f) * GetGraphics()->GetEngineParameters().WindowHeight;
	float paddingX = (PADDING / 1024.0f) * dx;
	float paddingY = (PADDING / 768.0f) * GetGraphics()->GetEngineParameters().WindowHeight;
	float xTemp = this->zX + startOffsetX;
	float yTemp = this->zY + startOffsetY;
	zSlotPositions[0] = Vector2(xTemp, yTemp);
	int row = 0;
	int col = 1;
	for(int i = 1; i < SLOTS; i++)
	{
		xTemp = this->zX + startOffsetX + (zSlotImageWidth + paddingX) * col;
		col++;
		yTemp = this->zY + startOffsetY + (zSlotImageHeight + paddingY) * row;

		if(col >= COL)
		{
			row++;
			col = 0;
		}

		zSlotPositions[i] = Vector2(xTemp, yTemp);
	}
	startOffsetY = (YOFFSETEQ / 768.0f) * GetGraphics()->GetEngineParameters().WindowHeight;
	xTemp = this->zX + (EQXPOS[0] / 1024.0f) * dx;
	zWeaponSlots[0] = Vector2(xTemp, this->zY + startOffsetY);

	xTemp = this->zX + (EQXPOS[1] / 1024.0f) * dx;
	zWeaponSlots[1] = Vector2(xTemp, this->zY + startOffsetY);

	xTemp = this->zX + (EQXPOS[2] / 1024.0f) * dx;
	zWeaponSlots[2] = Vector2(xTemp, this->zY + startOffsetY);

	for(int i = 0; i < SLOTS; i++)
	{
		InventorySlotGui* gui = new InventorySlotGui(zSlotPositions[i].x, zSlotPositions[i].y, zSlotImageWidth,
			zSlotImageHeight, Gui_Item_Data());
		this->zSlotGui.push_back(gui);
	}

	InventorySlotGui* gui = new InventorySlotGui(this->zWeaponSlots[0].x, this->zWeaponSlots[0].y, zSlotImageWidth,
		zSlotImageHeight, Gui_Item_Data());
	this->zWeaponSlotGui[ITEM_TYPE_WEAPON_MELEE] = gui;
	
	gui = new InventorySlotGui(this->zWeaponSlots[1].x, this->zWeaponSlots[1].y, zSlotImageWidth,
		zSlotImageHeight, Gui_Item_Data());
	this->zWeaponSlotGui[ITEM_TYPE_WEAPON_RANGED] = gui;

	gui = new InventorySlotGui(this->zWeaponSlots[2].x, this->zWeaponSlots[2].y, zSlotImageWidth,
		zSlotImageHeight, Gui_Item_Data());
	this->zWeaponSlotGui[ITEM_TYPE_PROJECTILE] = gui;

	this->zMaxWeight = 49.0f;
	this->zCurrentWeight = 0.0f;
	this->zNrOfItems = 0;
	this->zWeightText = NULL;
}

InventoryGui::~InventoryGui()
{
	for (auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
	for (auto it = this->zWeaponSlotGui.begin(); it != this->zWeaponSlotGui.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	if(this->zWeightText && GetGraphics()->IsRunning())
		GetGraphics()->DeleteText(this->zWeightText);
	if(this->zWeightText && !GetGraphics()->IsRunning())
		throw("Memory leaks!");
}

bool InventoryGui::AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
{
	if(gid.zCanStack) // If item can stack
	{
		bool found = false;
		Gui_Item_Data returnGid;
		for(int i = 0; i < SLOTS; i++) // See if the Item exists
		{
			returnGid = zSlotGui.at(i)->GetGid();
			if(gid.zType == returnGid.zType) // If item is the same type
			{
				if(gid.zSubType == returnGid.zSubType) // If item also is the same subtype
				{
					zSlotGui.at(i)->SetStacks(returnGid.zStacks + gid.zStacks); // Add to stacks
					i = SLOTS;
					return true;
				}
			}
		}
	}
	for(int i = 0; i < SLOTS; i++) // Find the next slot that is not blocked
	{
		if(!this->zSlotGui.at(i)->GetBlocked())
		{
			this->zSlotGui.at(i)->AddItemToSlot(gid, open, ge);
			i = SLOTS;
		}
	}
	return true;
}

bool InventoryGui::RemoveItemFromGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
{
	for(int i = 0; i < SLOTS; i++)
	{
		if(this->zSlotGui.at(i)->GetGid().zID == gid.zID) // If we find the item
		{
			bool removed = false;
			if(this->zSlotGui.at(i)->GetGid().zCanStack) // remove the stacks
			{
				this->zSlotGui.at(i)->SetStacks(this->zSlotGui.at(i)->GetGid().zStacks - gid.zStacks);

				if(this->zSlotGui.at(i)->GetGid().zStacks <= 0) // Remove item if it has less than or zero stacks
				{
					this->zSlotGui.at(i)->RemoveItemFromSlot(open, ge);
					removed = true;
				}
			}
			else // Remove the item if it cant stack
			{
				this->zSlotGui.at(i)->RemoveItemFromSlot(open, ge);
				removed = true;
			}
			if(removed)
			{
				int atPos = i;
				for(int k = i+1; k < SLOTS; k++) // Find the last one
				{
					if(this->zSlotGui.at(k)->GetBlocked())
					{
						atPos = k;
					}
					else
					{
						k = SLOTS;
					}
				}
				this->zSlotGui.at(i)->AddItemToSlot(this->zSlotGui.at(atPos)->GetGid(), open, ge); // Move last one to the one that we removed 
				this->zSlotGui.at(atPos)->RemoveItemFromSlot(open, ge);
			}

			i = SLOTS;
			return true;
		}
	}
	if(gid.zType == ITEM_TYPE_WEAPON_MELEE || gid.zType == ITEM_TYPE_WEAPON_MELEE || gid.zType == ITEM_TYPE_PROJECTILE)
		this->zWeaponSlotGui[gid.zType]->RemoveItemFromSlot(open, ge); // If item isn't in inventory

	return true;
	
}

bool InventoryGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if(x == this->zSlotGui.end())
			break;

		(*x)->AddToRenderer(ge);
	}
	for (auto x = this->zWeaponSlotGui.begin(); x != this->zWeaponSlotGui.end(); x++)
	{
		if(x == this->zWeaponSlotGui.end())
			break;

		x->second->AddToRenderer(ge);
	}
	if(!this->zWeightText)
	{
		float windowHeight = (float)(GetGraphics()->GetEngineParameters().WindowHeight);
		float dx = ((float)windowHeight * 4.0f) / 3.0f;

		this->zWeightText = ge->CreateText((MaloW::convertNrToString(
			this->zCurrentWeight) + ":" + MaloW::convertNrToString(this->zMaxWeight)).c_str(), 
			Vector2(this->zX + ((10.0f / 1024.0f) * dx), this->zY + ((10.0f / 768.0f) * windowHeight)), 1, "Media/Fonts/new");
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
	for (auto x = this->zWeaponSlotGui.begin(); x != this->zWeaponSlotGui.end(); x++)
	{
		x->second->RemoveFromRenderer(ge);
	}
	if(this->zWeightText)
	{
		ge->DeleteText(this->zWeightText);
	}
	return true;
}

Selected_Item_ReturnData InventoryGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();
	bool bCollision = false;

	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end() && !bCollision; x++)
		{
			if ((*x))
			{
				if((*x)->GetBlocked())
				{
					bCollision = (*x)->CheckCollision(mouseX, mouseY);
					if (bCollision)
					{
						if(mousePressed)
						{
							Selected_Item_ReturnData sir;
							sir.ID = (*x)->GetGid().zID;
							sir.type = (*x)->GetGid().zType;
							sir.inventory = 0;
							return sir;
						}
					}
				}
			}
		}
		for (auto x = this->zWeaponSlotGui.begin(); x != this->zWeaponSlotGui.end() && !bCollision; x++)
		{
			if (x->second)
			{
				bCollision = x->second->CheckCollision(mouseX, mouseY);
				if (bCollision)
				{
					if(mousePressed)
					{
						Selected_Item_ReturnData sir;
						sir.ID = x->second->GetGid().zID;
						if(this->zWeaponSlots[MELEE] == x->second->GetPosition())
							sir.type = MELEE;
						if(this->zWeaponSlots[RANGED] == x->second->GetPosition())
							sir.type = RANGED;
						if(this->zWeaponSlots[PROJECTILE] == x->second->GetPosition())
							sir.type = PROJECTILE;

						//sir.type = (*x)->GetType();
						sir.inventory = 1;
						return sir;
					}
				}
			}
		}
	}
	Selected_Item_ReturnData sir;
	sir.ID = -1;
	sir.type = -1;
	sir.inventory = -1;
	return sir;
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

void InventoryGui::EquipItem( const Gui_Item_Data gid, bool guiOpen )
{
	if(!this->zWeaponSlotGui.at(gid.zType)->GetBlocked())
	{
		this->zWeaponSlotGui.at(gid.zType)->AddItemToSlot(gid, guiOpen, GetGraphics());
	}
}

void InventoryGui::UnEquipItem(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
{
	if(this->zWeaponSlotGui.at(gid.zType)->GetBlocked())
	{
		this->zWeaponSlotGui.at(gid.zType)->RemoveItemFromSlot(open, GetGraphics());
	}
}

void InventoryGui::Resize(float windowWidth, float windowHeight, float dx)
{
	// Calcs that is needed for resize
	float startOffsetX = (XOFFSETINV / 1024.0f) * dx;
	float startOffsetY = (YOFFSETINV / 768.0f) * windowHeight;
	this->zSlotImageWidth = (SLOTIMAGEWIDTH / 1024.0f) * dx;
	this->zSlotImageHeight = (SLOTIMAGEHEIGHT / 768.0f) * windowHeight;
	float paddingX = (PADDING / 1024.0f) * dx;
	float paddingY = (PADDING / 768.0f) * windowHeight;
	float xTemp = this->zX + startOffsetX;
	float yTemp = this->zY + startOffsetY;
	Vector2 newSlotPositions[SLOTS];
	newSlotPositions[0] = Vector2(xTemp, yTemp);
	int row = 0;
	int col = 1;

	
	// Adding slot positions to lookup table
	for(int i = 1; i < SLOTS; i++)
	{
		xTemp = this->zX + startOffsetX + (zSlotImageWidth + paddingX) * col;
		col++;
		yTemp = this->zY + startOffsetY + (zSlotImageHeight + paddingY) * row;

		if(col >= COL)
		{
			row++;
			col = 0;
		}

		newSlotPositions[i] = Vector2(xTemp, yTemp);
	}
	// Moving and resizeing
	InventorySlotGui* isg;
	for(unsigned int i = 0; i < this->zSlotGui.size(); i++)
	{
		isg = zSlotGui.at(i);
		for(int k = 0; k < SLOTS; k++)
		{
			if(isg->GetPosition() == zSlotPositions[k])
			{
				isg->SetPosition(newSlotPositions[k]);
				isg->SetDimension(Vector2(zSlotImageWidth, zSlotImageHeight));
			}
		}
	}
	for(int i = 0; i < SLOTS; i++)
		zSlotPositions[i] = newSlotPositions[i];

	Vector2 newWeaponSlots[WEAPONSLOTS];

	startOffsetY = (YOFFSETEQ / 768.0f) * windowHeight;
	xTemp = this->zX + (EQXPOS[0] / 1024.0f) * dx;
	newWeaponSlots[0] = Vector2(xTemp, this->zY + startOffsetY);

	xTemp = this->zX + (EQXPOS[1] / 1024.0f) * dx;
	newWeaponSlots[1] = Vector2(xTemp, this->zY + startOffsetY);

	xTemp = this->zX + (EQXPOS[2] / 1024.0f) * dx;
	newWeaponSlots[2] = Vector2(xTemp, this->zY + startOffsetY);

	for(unsigned int i = 0; i < this->zWeaponSlotGui.size(); i++)
	{
		for(int k = 0; k < WEAPONSLOTS; k++)
		{
			isg = this->zWeaponSlotGui.at(i);
			if(isg->GetPosition() == zWeaponSlots[k])
			{
				isg->SetPosition(newWeaponSlots[k]);
				isg->SetDimension(Vector2(zSlotImageWidth, zSlotImageHeight));
			}
		}
	}
	for(int k = 0; k < WEAPONSLOTS; k++)
		zWeaponSlots[k] = newWeaponSlots[k];
}