#include "LootInventoryGui.h"
#include <Safe.h>


#define SLOTIMAGEWIDTH 50.0f
#define SLOTIMAGEHEIGHT 50.0f
#define PADDING 12.0f
#define XOFFSETINV 32.0f
#define YOFFSETINV 75.0f

LootInventoryGui::LootInventoryGui()
{
	zPressed = false;
}

LootInventoryGui::LootInventoryGui(float x, float y, float width, float height, std::string textureName) 
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

	for(int i = 0; i < SLOTS; i++)
	{
		InventorySlotGui* gui = new InventorySlotGui(zSlotPositions[i].x, zSlotPositions[i].y, zSlotImageWidth,
			zSlotImageHeight, Gui_Item_Data());
		this->zSlotGui.push_back(gui);
	}

	this->zActorLoot = 0;
	this->zNrOfItems = 0;
}

LootInventoryGui::~LootInventoryGui()
{
	for (auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

bool LootInventoryGui::AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
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

bool LootInventoryGui::RemoveItemFromGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge)
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

	return true;
	
}

bool LootInventoryGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if(x == this->zSlotGui.end())
			break;

		(*x)->AddToRenderer(ge);
	}
	return true;

}

bool LootInventoryGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		(*x)->RemoveFromRenderer(ge);
	}
	return true;
}

Selected_Item_ReturnData LootInventoryGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
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
							sir.gid = (*x)->GetGid();
							sir.inventory = 1;
							return sir;
						}
					}
				}
			}
		}
	}
	Selected_Item_ReturnData sir;
	sir.gid.zID = -1;
	sir.gid.zType = -1;
	sir.inventory = -1;
	return sir;
}

void LootInventoryGui::HideGui()
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

void LootInventoryGui::ShowGui()
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

void LootInventoryGui::FadeOut(float value)
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

std::string LootInventoryGui::GetImageName( unsigned int position )
{
	std::string ret;
	this->zSlotGui[position]->GetTextureName(ret);
	return ret;
}

void LootInventoryGui::Resize(float windowWidth, float windowHeight, float dx)
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
}

void LootInventoryGui::Reset( bool open )
{
	for (unsigned int i = 0; i < this->zSlotGui.size(); i++)
	{
		this->zSlotGui.at(i)->RemoveItemFromSlot(open, GetGraphics());
	}

	this->zNrOfItems = 0;
	this->zActorLoot = 0;
}
