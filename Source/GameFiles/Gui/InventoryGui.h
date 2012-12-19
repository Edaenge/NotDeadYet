/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"
#include "GameFiles/Gui/InventorySlotGui.h"
#include <vector>

class InventoryGui : public GuiElement
{
public:
	InventoryGui();
	InventoryGui(float x, float y, float width, float height, std::string textureName);
	virtual ~InventoryGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	bool CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

private:
	Vector2 zNrOfSlots;
	std::vector<iImage*> zSlotGui;
};