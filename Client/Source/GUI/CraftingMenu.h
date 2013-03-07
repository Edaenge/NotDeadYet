#pragma once

#include "InventorySlotGui.h"
#include "Item.h"

#define ROWS 3
#define COL 4
#define SLOTS (ROWS * COL)

class CraftingMenu : public GuiElement
{
private:
	// Public variables
	float zX;
	float zY;

	std::vector<InventorySlotGui*> zSlotGui;
	Vector2 zSlotPositions[SLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;

	int zNrOfItems;

	//Private Functions.
	
	
public:
	//Public Functions.
	CraftingMenu();
	CraftingMenu(float x, float y, float width, float height, std::string textureName);
	virtual ~CraftingMenu();

	//Call this before adding to renderer.
	void UpdateCrafting(std::vector<InventorySlotGui*> inventory);

	//Inherited from GuiElement.
	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	void Reset(bool open);
	void Resize(float windowWidth, float windowHeight, float dx);
};