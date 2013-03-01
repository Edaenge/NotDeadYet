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
#define WEAPONSLOTS 3
#define SLOTS (ROWS * COL)

enum EQUIPITEMTYPE
{
	MELEE,
	RANGED,
	PROJECTILE
};
class InventoryGui : public GuiElement
{
public:
	InventoryGui();
	InventoryGui(float x, float y, float width, float height, std::string textureName);
	virtual ~InventoryGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	Selected_Item_ReturnData CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	bool AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge);
	bool RemoveItemFromGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge);
	void EquipItem(const Gui_Item_Data gid, bool guiOpen);
	void UnEquipItem(Gui_Item_Data gid, bool open, GraphicsEngine* ge);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);

	void Resize(float windowWidth, float windowHeight, float dx);
private:
	std::vector<InventorySlotGui*> zSlotGui;
	std::map<int, InventorySlotGui*> zWeaponSlotGui;
	Vector2 zSlotPositions[SLOTS];
	Vector2 zWeaponSlots[WEAPONSLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;

	float zMaxWeight;
	float zCurrentWeight;
	int zNrOfItems;
	iText* zWeightText;
};