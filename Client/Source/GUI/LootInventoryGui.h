/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GuiElement.h"
#include "InventorySlotGui.h"
#include "Item.h"
#include <vector>
#include <map>

#define ROWS 7
#define COL 7
#define SLOTS (ROWS * COL)

class LootInventoryGui : public GuiElement
{
public:
	LootInventoryGui();
	LootInventoryGui(float x, float y, float width, float height, std::string textureName);
	virtual ~LootInventoryGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	Selected_Item_ReturnData CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	bool AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge);
	bool RemoveItemFromGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);

	void SetLootActor(unsigned int ID){ this->zActorLoot = ID; }
	unsigned int GetLootActor(){ return this->zActorLoot; }
	void Reset(bool open);

	void Resize(float windowWidth, float windowHeight, float dx);
private:
	std::vector<InventorySlotGui*> zSlotGui;
	Vector2 zSlotPositions[SLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;

	unsigned int zActorLoot;

	int zNrOfItems;
};