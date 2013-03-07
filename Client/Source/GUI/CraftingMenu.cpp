#include "CraftingMenu.h"
#include "Safe.h"
#include "CraftingReader.h"

#define SLOTIMAGEWIDTH 50.0f
#define SLOTIMAGEHEIGHT 50.0f
#define PADDING 12.0f
#define XOFFSETINV 32.0f
#define YOFFSETINV 75.0f

CraftingMenu::CraftingMenu() : GuiElement()
{
	this->zX;
	this->zY;

	for(int i = 0; i < SLOTS; i++)
	{
		this->zSlotGui.push_back(NULL);
	}

	this->zSlotImageWidth = 0.0f;
	this->zSlotImageHeight = 0.0f;
}

CraftingMenu::CraftingMenu( float x, float y, float width, float height, std::string textureName )
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
	for(int i = col; i < SLOTS; i++)
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

	this->zNrOfItems = 0;
}

CraftingMenu::~CraftingMenu()
{
	for (auto it = this->zSlotGui.begin(); it != this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

void CraftingMenu::UpdateCrafting(std::vector<InventorySlotGui*> inventory)
{

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
	return true;
}
