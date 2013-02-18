/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GuiElement.h"
#include "InventorySlotGui.h"
#include <vector>

static const int ROWS = 7;
static const int COL = 7;
static const int WEAPONSLOTS = 3;
static const int SLOTS = ROWS * COL;

struct Gui_Item_Data
{
	Gui_Item_Data()
	{
		this->zID = 0;
		this->zType = 0;
		this->zWeight = 0;
		this->zStacks = 0;
		this->zSubType = 0;
		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}
	Gui_Item_Data(const unsigned int ID, const int weight, const int stack, const std::string& name, 
		const std::string& filePath, const std::string& description, const int Type, const int subType)
	{
		this->zID = ID;
		this->zType = Type;
		this->zName = name;
		this->zStacks = stack;
		this->zWeight = weight;
		this->zSubType = subType;
		this->zFilePath = filePath;
		this->zDescription = description;
	}
	unsigned int zID;
	int zType;
	int zWeight;
	int zStacks;
	int zSubType;
	std::string zName;
	std::string zFilePath;
	std::string zDescription;

	bool zPressed;
};
struct Selected_Item_ReturnData
{
	int ID;
	int type;
	int inventory;
};
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
	void EquipItem(int type, const Gui_Item_Data gid, bool guiOpen);
	void UnEquipItem(const int ID, int stacks);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);

	void Resize(float windowWidth, float windowHeight, float dx);
private:
	std::vector<InventorySlotGui*> zSlotGui;
	std::vector<InventorySlotGui*> zWeaponSlotGui;
	Vector2 zSlotPositions[SLOTS];
	Vector2 zWeaponSlots[WEAPONSLOTS];
	float zSlotImageWidth;
	float zSlotImageHeight;

	float zMaxWeight;
	float zCurrentWeight;
	iText* zWeightText;
};