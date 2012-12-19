/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"

class InventorySlotGui : public GuiElement
{
public:
	InventorySlotGui();
	InventorySlotGui(float x, float y, float width, float height, std::string textureName);
	virtual ~InventorySlotGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	bool CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);

private:
	iImage* zSlotImage;
};