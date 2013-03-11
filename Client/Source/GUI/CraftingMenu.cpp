#include "CraftingMenu.h"
#include "Safe.h"
#include "CraftingReader.h"

#define SLOTIMAGEWIDTH 50.0f
#define SLOTIMAGEHEIGHT 50.0f
#define PADDING 12.0f
#define XOFFSETINV 32.0f
#define YOFFSETINV 82.0f

CraftingMenu::CraftingMenu() : GuiElement()
{
	InitCraftingRecipes();
	this->zX;
	this->zY;

	for(int i = 0; i < CRAFTSLOTS; i++)
	{
		this->zSlotGui.push_back(NULL);
	}

	this->zSlotImageWidth = 0.0f;
	this->zSlotImageHeight = 0.0f;

	this->zRecipeNote = NULL;
	this->zSelectedGid = Gui_Item_Data();
	this->zNrOfItems = 0;
}

CraftingMenu::CraftingMenu( float x, float y, float width, float height, std::string textureName )
	: GuiElement(x, y, width, height, textureName)
{
	float windowHeight = (float)(GetGraphics()->GetEngineParameters().WindowHeight);
	float dx = ((float)windowHeight * 4.0f) / 3.0f;

	this->zX = this->GetPosition().x;
	this->zY = this->GetPosition().y;

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
	for(int i = 1; i < CRAFTSLOTS; i++)
	{
		xTemp = this->zX + startOffsetX + (zSlotImageWidth + paddingX) * col;
		col++;
		yTemp = this->zY + startOffsetY + (zSlotImageHeight + paddingY) * row;

		if(col >= CRAFTCOL)
		{
			row++;
			col = 0;
		}

		zSlotPositions[i] = Vector2(xTemp, yTemp);
	}

	for(int i = 0; i < CRAFTSLOTS; i++)
	{
		InventorySlotGui* gui = new InventorySlotGui(zSlotPositions[i].x, zSlotPositions[i].y, zSlotImageWidth,
			zSlotImageHeight, Gui_Item_Data());
		this->zSlotGui.push_back(gui);
	}

	this->zNrOfItems = 0;
	this->zRecipeNote = NULL;
	this->zSelectedGid = Gui_Item_Data();
}

CraftingMenu::~CraftingMenu()
{
	for (auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

Gui_Item_Data CraftingMenu::CheckCollision( float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge )
{

	for(auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
	{
		if((*it)->CheckCollision(mouseX, mouseY))
		{
			if(mousePressed)
			{
				 return (*it)->GetGid();
			}
			else
			{
				if(this->zSelectedGid.zSubType != (*it)->GetGid().zSubType || this->zSelectedGid.zType != (*it)->GetGid().zType)
				{
					this->zSelectedGid = (*it)->GetGid();
					for(auto i = this->zCanCraftList.cbegin(); i != this->zCanCraftList.cend(); i++)
					{
						if((*i)->subType == this->zSelectedGid.zSubType && (*i)->type == this->zSelectedGid.zType)
						{
							CraftingReader* CraftRead = GetCraftingRecipes();
							if(this->zRecipeNote)
							{
								delete this->zRecipeNote;
								this->zRecipeNote = NULL;
							}
							this->zRecipeNote = new RecipeNoteGui(GetGraphics()->GetKeyListener()->GetMousePosition(),
								CraftRead->GetMaterialReq(*i));

							return Gui_Item_Data();
						}
					}
				}
			}
			return Gui_Item_Data();
		}
	}
	if(this->zRecipeNote)
	{
		this->zSelectedGid = Gui_Item_Data();
		delete this->zRecipeNote;
		this->zRecipeNote = NULL;
	}
	return Gui_Item_Data();
}

void CraftingMenu::UpdateCrafting(std::vector<InventorySlotGui*> inventory)
{

	GraphicsEngine* ge = GetGraphics();
	CraftingReader* CraftRead = GetCraftingRecipes();
	std::vector<CraftedTypes*> CraftTypes = CraftRead->GetCraftableItemTypes(NULL);
	this->zCanCraftList.clear();
	bool canCraft;
	bool materialFound;
	//Cycle by all craftables.
	for(auto it = CraftTypes.cbegin(); it != CraftTypes.cend(); it++)
	{
		const std::map<unsigned int, unsigned int>* materialReq = CraftRead->GetMaterialReq(*it);
		canCraft = true;
		
		// Cycle by all the materials that is needed for the item.
		for(auto subType = materialReq->cbegin(); subType != materialReq->cend(); subType++)
		{
			materialFound = false;
			//See if the item is in the inventory. If not set canCraft to false
			for(auto item = inventory.begin(); item != inventory.end(); item++)
			{
				if((*item)->GetGid().zSubType == subType->first)
					if((*item)->GetGid().zStacks >= subType->second)
					{
						materialFound = true;
						break;
					}
			}
			if(!materialFound)
			{
				canCraft = false;
				break;
			}
		}
		if(canCraft)
			this->zCanCraftList.push_back(*it); // Push onto the craftable vector
	}
	this->Reset(false); // Open is false because this will only be called before you add to renderer.
	for(auto craftItem = this->zCanCraftList.begin(); craftItem != this->zCanCraftList.end(); craftItem++)
	{
		for(auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
		{
			if(!(*it)->GetBlocked()) // Look for the next slot that isnt blocked
			{
				Gui_Item_Data gid = Gui_Item_Data(0, (*craftItem)->type, (*craftItem)->subType, 
					0, 0, 0, false, "Craft", (*craftItem)->filePath, "CraftableItem");
				(*it)->AddItemToSlot(gid, false, ge);
				break;
			}
		}
	}

}

bool CraftingMenu::AddToRenderer( GraphicsEngine* ge )
{
	GuiElement::AddToRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x != this->zSlotGui.end(); x++)
	{
		(*x)->AddToRenderer(ge);
	}
	return true;
}

bool CraftingMenu::RemoveFromRenderer( GraphicsEngine* ge )
{
	GuiElement::RemoveFromRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		(*x)->RemoveFromRenderer(ge);
	}
	if(this->zRecipeNote)
	{
		SAFE_DELETE(this->zRecipeNote);
		this->zRecipeNote = NULL;
	}
	return true;
}

void CraftingMenu::Reset( bool open )
{
	for (unsigned int i = 0; i < this->zSlotGui.size(); i++)
	{
		this->zSlotGui.at(i)->RemoveItemFromSlot(open, GetGraphics());
	}

	this->zNrOfItems = 0;
}

void CraftingMenu::Resize( float windowWidth, float windowHeight, float dx )
{
	// CTRL + C -> CTRL + V from looting gui... may not work. TODO Check this.

	// Calcs that is needed for resize
	float startOffsetX = (XOFFSETINV / 1024.0f) * dx;
	float startOffsetY = (YOFFSETINV / 768.0f) * windowHeight;
	this->zSlotImageWidth = (SLOTIMAGEWIDTH / 1024.0f) * dx;
	this->zSlotImageHeight = (SLOTIMAGEHEIGHT / 768.0f) * windowHeight;
	float paddingX = (PADDING / 1024.0f) * dx;
	float paddingY = (PADDING / 768.0f) * windowHeight;
	float xTemp = this->zX + startOffsetX;
	float yTemp = this->zY + startOffsetY;
	Vector2 newSlotPositions[CRAFTSLOTS];
	newSlotPositions[0] = Vector2(xTemp, yTemp);
	int row = 0;
	int col = 1;


	// Adding slot positions to lookup table
	for(int i = 1; i < CRAFTSLOTS; i++)
	{
		xTemp = this->zX + startOffsetX + (zSlotImageWidth + paddingX) * col;
		col++;
		yTemp = this->zY + startOffsetY + (zSlotImageHeight + paddingY) * row;

		if(col >= CRAFTCOL)
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
		for(int k = 0; k < CRAFTSLOTS; k++)
		{
			if(isg->GetPosition() == zSlotPositions[k])
			{
				isg->SetPosition(newSlotPositions[k]);
				isg->SetDimension(Vector2(zSlotImageWidth, zSlotImageHeight));
			}
		}
	}
	for(int i = 0; i < CRAFTSLOTS; i++)
		zSlotPositions[i] = newSlotPositions[i];
}
