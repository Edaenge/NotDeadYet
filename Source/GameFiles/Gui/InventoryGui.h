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
		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}
	Gui_Item_Data(const int id, const std::string& name, const std::string& filePath, const std::string& description)
	{
		this->zID = id;
		this->zName = name;
		this->zFilePath = filePath;
		this->zDescription = description;
	}
	int zID;	
	std::string zName;
	std::string zFilePath;
	std::string zDescription;

	bool zPressed;
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
	bool AddItemToGui(Gui_Item_Data gid, GraphicsEngine* ge);
	bool RemoveItemFromGui(const unsigned int position);

	void FadeOut(float value);
	void ShowGui();
	void HideGui();
	std::string GetImageName(unsigned int position);
private:
	Vector2 zNrOfSlots;
	std::vector<InventorySlotGui*> zSlotGui;
};