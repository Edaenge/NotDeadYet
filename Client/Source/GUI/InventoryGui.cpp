#include "InventoryGui.h"
#include <Safe.h>


static const float SLOTIMAGEWIDTH = 50.0f;
static const float SLOTIMAGEHEIGHT = 50.0f;
static const float PADDING = 12.0f;
static const float XOFFSETINV = 32.0f;
static const float YOFFSETINV = 206.0f;
static const float YOFFSETEQ = 62.0f;
static const float EQXPOS[] = {94, 280, 342};

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

	this->zMaxWeight = 49.0f;
	this->zCurrentWeight = 0.0f;
	this->zWeightText = NULL;


}

InventoryGui::~InventoryGui()
{
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
	for (auto it = this->zWeaponSlotGui.begin(); it < this->zWeaponSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

bool InventoryGui::AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
{
	if(this->zWeightText)
		this->zCurrentWeight = this->zCurrentWeight + (gid.zWeight * gid.zStacks);
	this->zWeightText->SetText((MaloW::convertNrToString(this->zCurrentWeight) + "/" + 
		MaloW::convertNrToString(this->zMaxWeight)).c_str());

	int size = this->zSlotGui.size();
	bool stacks = false;
	if(size >=  SLOTS)
		return false;
	if(gid.zStacks > 0)
	{
		for(unsigned int i = 0; i < this->zSlotGui.size(); i++)
		{
			if(this->zSlotGui.at(i)->GetType() == gid.zType && this->zSlotGui.at(i)->GetSubType() == gid.zSubType)
			{
				if(this->zSlotGui.at(i)->GetStacks() > 0)
				{
					this->zSlotGui.at(i)->SetStacks(this->zSlotGui.at(i)->GetStacks() + gid.zStacks);
					stacks = true;
					return true;
				}
			}
		}
	}
	InventorySlotGui* gui = new InventorySlotGui(zSlotPositions[size].x, zSlotPositions[size].y, zSlotImageWidth
		, zSlotImageHeight, gid.zFilePath, gid.zID, gid.zType, gid.zSubType, gid.zStacks);
	this->zSlotGui.push_back(gui);

	if(open)
		gui->AddToRenderer(ge);

	return true;
}

bool InventoryGui::RemoveItemFromGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
{
	this->zCurrentWeight -= gid.zStacks * gid.zWeight;
	if(this->zWeightText)
		this->zWeightText->SetText((MaloW::convertNrToString(this->zCurrentWeight) + "/" + MaloW::convertNrToString(this->zMaxWeight)).c_str());

	int size = zSlotGui.size();
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		if ((*it)->GetID() == gid.zID)
		{
			if((*it)->GetStacks() > 0)
			{
				(*it)->SetStacks((*it)->GetStacks() - gid.zStacks);
				if((*it)->GetStacks() > 0)
					return true;
			}
			if(this->zSlotGui.end()-- != it)
			{
				for(auto i = this->zSlotGui.begin(); i < this->zSlotGui.end(); i++)
				{
					if((*i)->GetPosition().x == zSlotPositions[size-1].x && (*i)->GetPosition().y == zSlotPositions[size].y)
					{
						(*i)->SetPosition((*it)->GetPosition());
					}
				}
			}
			(*it)->RemoveFromRenderer(GetGraphics());
			this->zSlotGui.erase(it);

			return true;
		}
	}
	for (auto it = this->zWeaponSlotGui.begin(); it < this->zWeaponSlotGui.end(); it++)
	{
		if ((*it)->GetID() == gid.zID)
		{
			if((*it)->GetStacks() > 0)
			{
				(*it)->SetStacks((*it)->GetStacks() - gid.zStacks);
				if((*it)->GetStacks() > 0)
					return true;
			}
			(*it)->RemoveFromRenderer(GetGraphics());
			this->zWeaponSlotGui.erase(it);

			return true;
		}
	}
	return false;
	
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
	for (auto x = this->zWeaponSlotGui.begin(); x < this->zWeaponSlotGui.end(); x++)
	{
		if(x == this->zWeaponSlotGui.end())
			break;

		(*x)->AddToRenderer(ge);
	}
	if(!this->zWeightText)
	{
		this->zWeightText = ge->CreateText((MaloW::convertNrToString(
			this->zCurrentWeight) + "/" + MaloW::convertNrToString(this->zMaxWeight)).c_str(), 
			Vector2(100,100), 1, "Media/Fonts/1.png");
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
	for (auto x = this->zWeaponSlotGui.begin(); x < this->zWeaponSlotGui.end(); x++)
	{
		(*x)->RemoveFromRenderer(ge);
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
				bCollision = (*x)->CheckCollision(mouseX, mouseY);
				if (bCollision)
				{
					if(mousePressed)
					{
						Selected_Item_ReturnData sir;
						sir.ID = (*x)->GetID();
						sir.type = (*x)->GetType();
						sir.inventory = 0;
						return sir;
					}
				}
			}
		}
		for (auto x = this->zWeaponSlotGui.begin(); x < this->zWeaponSlotGui.end() && !bCollision; x++)
		{
			if ((*x))
			{
				bCollision = (*x)->CheckCollision(mouseX, mouseY);
				if (bCollision)
				{
					if(mousePressed)
					{
						Selected_Item_ReturnData sir;
						sir.ID = (*x)->GetID();
						if(this->zWeaponSlots[MELEE] == (*x)->GetPosition())
							sir.type = MELEE;
						if(this->zWeaponSlots[RANGED] == (*x)->GetPosition())
							sir.type = RANGED;
						if(this->zWeaponSlots[PROJECTILE] == (*x)->GetPosition())
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

void InventoryGui::EquipItem( int type, const Gui_Item_Data gid, bool guiOpen )
{
	int size = zWeaponSlotGui.size();
	for(int i = 0; i < size; i++)
	{
		if(zWeaponSlotGui.at(i)->GetType() == type)
		{
			return;
		}
	}
	InventorySlotGui* gui = new InventorySlotGui(zWeaponSlots[type].x, zWeaponSlots[type].y, zSlotImageWidth, 
		zSlotImageHeight, gid.zFilePath, gid.zID, type, gid.zSubType, gid.zStacks);

	if(guiOpen)
		gui->AddToRenderer(GetGraphics());

	zWeaponSlotGui.push_back(gui);
	
}

void InventoryGui::UnEquipItem( const int ID, int stacks )
{
	int size = zWeaponSlotGui.size();
	for(int i = 0; i < size; i++)
	{
		if(zWeaponSlotGui.at(i)->GetID() == ID)
		{
			InventorySlotGui* InvGui = this->zWeaponSlotGui.at(i);
			InvGui->RemoveFromRenderer(GetGraphics());
			this->zWeaponSlotGui.erase(zWeaponSlotGui.begin() + i);
			delete InvGui;
			InvGui = NULL;
			return;
		}
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