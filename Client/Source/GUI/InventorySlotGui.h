/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GuiElement.h"

class InventorySlotGui : public GuiElement
{
public:
	InventorySlotGui();
	InventorySlotGui(float x, float y, float width, float height, std::string textureName, int ID, int Type, int Stacks);
	virtual ~InventorySlotGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	virtual void SetPosition(Vector2 pos);
	virtual void SetDimension(Vector2 dim);

	bool CheckCollision(float mouseX, float mouseY);
	
	void FadeOut(float value);
	void ShowGui();
	void HideGui();

	int GetID(){ return this->zID; }

	int GetType(){ return this->zType; }
	int GetStacks(){ return this->zStacks; }

	void SetStacks(int stacks) { this->zStacks = stacks; if(this->zStackText) this->zStackText->SetText(MaloW::convertNrToString((float)this->zStacks).c_str()); }
private:
	iImage* zSlotImage;
	iText* zStackText;
	int zID;
	int zType;
	int zStacks;
};