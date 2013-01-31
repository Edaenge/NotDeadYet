/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"
#include "GameFiles/Gui/InventorySlotGui.h"
#include <vector>

struct Gui_Item_Data
{
	Gui_Item_Data()
	{
		this->zID = -1;
		this->zType = 0;
		this->zWeight = 0;
		this->zStacks = 0;
		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}
	Gui_Item_Data(const long ID, const int weight, const int stack, const std::string& name, 
		const std::string& filePath, const std::string& description, const int type)
	{
		this->zID = ID;
		this->zType = type;
		this->zName = name;
		this->zStacks = stack;
		this->zWeight = weight;
		this->zFilePath = filePath;
		this->zDescription = description;
	}
	long zID;
	int zType;
	int zWeight;
	int zStacks;
	std::string zName;
	std::string zFilePath;
	std::string zDescription;

	bool zPressed;
};

const int ROWS = 7;
const int COL = 7;
const int WEAPONSLOTS = 3;
const int SLOTS = ROWS * COL;

enum EQUIPITEMTYPE{
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

	int CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	bool AddItemToGui(Gui_Item_Data gid, bool open, GraphicsEngine* ge);
	bool RemoveItemFromGui(const int ID, int stacks);
	void EquipItem(int type, const Gui_Item_Data gid);
	void UnEquipItem(const int ID, int stacks);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);
private:
	std::vector<InventorySlotGui*> zSlotGui;
	std::vector<InventorySlotGui*> zWeaponSlotGui;
	Vector2 zSlotPositions[SLOTS];
	Vector2 zWeaponSlots[WEAPONSLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;
};