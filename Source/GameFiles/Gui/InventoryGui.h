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
		this->zWeight = 0;
		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}
	Gui_Item_Data(const int ID, const int weight, const std::string& name, const std::string& filePath, const std::string& description)
	{
		this->zID = ID;
		this->zName = name;
		this->zWeight = weight;
		this->zFilePath = filePath;
		this->zDescription = description;
	}
	int zID;
	int zWeight;
	std::string zName;
	std::string zFilePath;
	std::string zDescription;

	bool zPressed;
};

const int ROWS = 7;
const int COL = 7;
const int SLOTS = ROWS * COL;

class InventoryGui : public GuiElement
{
public:
	InventoryGui();
	InventoryGui(float x, float y, float width, float height, std::string textureName);
	virtual ~InventoryGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	int CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	bool AddItemToGui(Gui_Item_Data gid, GraphicsEngine* ge);
	bool RemoveItemFromGui(const unsigned int ID);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);
private:
	std::vector<InventorySlotGui*> zSlotGui;
	Vector2 zSlotPositions[SLOTS];
};