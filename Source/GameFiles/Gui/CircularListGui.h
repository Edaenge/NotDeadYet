/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"
#include "Items/Item.h"

enum CIRCMENU
{
	USE,
	CRAFT,
	DROP,
	UNEQUIP,
	EQUIP,
	NOTHING
};

static const float CIRCLISTRADIUS = 200.0f;

class CircularListGui : public GuiElement
{
public:
	CircularListGui();
	CircularListGui(float x, float y, float width, float height, std::string textureName, int options[]);
	virtual ~CircularListGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
	virtual void SetPosition(Vector2 pos);

	int CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	//void SetItemTexture(const std::string& itemTexName){this->zItemImageName = itemTexName;}
	void FadeOut(float value);
	void ShowGui();
	void HideGui();

	void Resize(int width, int height, int dx);
	void Adjust(int type, int inventory);
private:
	int zOptions[4];
	Vector2 zTextImagePos[4];
	iImage* zTextImages[4];
};