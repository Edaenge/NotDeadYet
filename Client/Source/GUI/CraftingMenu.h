#pragma once
#include "CraftingReader.h"
#include "InventorySlotGui.h"
#include "RecipeNoteGui.h"
#include "Item.h"

#define CRAFTROWS 3
#define CRAFTCOL 4
#define CRAFTSLOTS (CRAFTROWS * CRAFTCOL)

class CraftingMenu : public GuiElement
{
private:
	// Public variables
	float zX;
	float zY;

	std::vector<CraftedTypes*> zCanCraftList;
	std::vector<InventorySlotGui*> zSlotGui;
	Vector2 zSlotPositions[CRAFTSLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;
	RecipeNoteGui* zRecipeNote;
	Gui_Item_Data zSelectedGid;

	int zNrOfItems;

	//Private Functions.
	
public:
	//Public Functions.
	CraftingMenu();
	CraftingMenu(float x, float y, float width, float height, std::string textureName);
	virtual ~CraftingMenu();

	// Checks what to craft
	Selected_Item_ReturnData CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

	//Call this before adding to renderer.
	void UpdateCrafting(std::vector<InventorySlotGui*> inventory);

	//Inherited from GuiElement.
	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	void Reset(bool open);
	void Resize(float windowWidth, float windowHeight, float dx);
};