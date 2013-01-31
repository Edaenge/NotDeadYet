#include "GameFiles/Gui/InventoryGui.h"
#include "../../MaloWLib/Safe.h"

InventoryGui::InventoryGui()
{
	zPressed = false;
}

InventoryGui::InventoryGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	float windowWidth = (float)(GetGraphics()->GetEngineParameters()->windowWidth);
	float windowHeight = (float)(GetGraphics()->GetEngineParameters()->windowHeight);
	float dx = ((float)windowHeight * 4.0f) / 3.0f;

	float startOffsetX = (32.0f / 1024.0f) * dx;
	float startOffsetY = (206.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	this->zSlotImageWidth = (50.0f / 1024.0f) * dx;
	this->zSlotImageHeight = (50.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	float paddingX = (12.0f / 1024.0f) * dx;
	float paddingY = (12.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
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
	startOffsetY = (62.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	xTemp = this->zX + (94.0f / 1024.0f) * dx;
	zWeaponSlots[0] = Vector2(xTemp, startOffsetY);

	xTemp = this->zX + (208.0f / 1024.0f) * dx;
	zWeaponSlots[1] = Vector2(xTemp, startOffsetY);

	xTemp = this->zX + (342.0f / 1024.0f) * dx;
	zWeaponSlots[2] = Vector2(xTemp, startOffsetY);
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
	int size = this->zSlotGui.size();
	bool stacks = false;
	if(size >=  SLOTS)
		return false;
	if(gid.zStacks > 0)
	{
		for(unsigned int i = 0; i < this->zSlotGui.size(); i++)
		{
			if(this->zSlotGui.at(i)->GetType() == gid.zType)
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
		, zSlotImageHeight, gid.zFilePath, gid.zID, gid.zType, gid.zStacks);
	this->zSlotGui.push_back(gui);

	if(open)
		gui->AddToRenderer(ge);

	return true;
}

bool InventoryGui::RemoveItemFromGui(const int ID, int stacks)
{
	int size = zSlotGui.size();
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		if ((*it)->GetID() == ID)
		{
			if((*it)->GetStacks() > 0)
			{
				(*it)->SetStacks((*it)->GetStacks() - stacks);
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
					if(mousePressed)
						return (*x)->GetID();
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

void InventoryGui::EquipItem( int type, const Gui_Item_Data gid )
{
	int size = zWeaponSlotGui.size();
	for(int i = 0; i < size; i++)
	{
		if(zWeaponSlotGui.at(i)->GetType() == type)
		{
			return;
		}
	}
	InventorySlotGui* gui = new InventorySlotGui(zWeaponSlots[size].x, zWeaponSlots[size].y, zSlotImageWidth
		, zSlotImageHeight, gid.zFilePath, gid.zID, gid.zType, gid.zStacks);

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
			this->zWeaponSlotGui.erase(zWeaponSlotGui.begin() + i);
			delete InvGui;
			InvGui = NULL;
		}
	}
}
