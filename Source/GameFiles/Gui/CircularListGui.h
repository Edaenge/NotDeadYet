/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"

enum CIRCMENU
{
	USE,
	CRAFT,
	DROP,
	NOTHING
};

static const float CIRCLISTRADIUS = 200.0f;

class CircularListGui : public GuiElement
{
public:
	CircularListGui();
	CircularListGui(float x, float y, float width, float height, std::string textureName);
	virtual ~CircularListGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	int CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
	void SetItemTexture(const std::string& itemTexName){this->zItemImageName = itemTexName;}
	void FadeOut(float value);
	void ShowGui();
	void HideGui();

	void Resize(int width, int height, int dx);
private:
	std::string zItemImageName;
	float zItemX;
	float zItemY;
	float zItemWidth;
	float zItemHeight;
	iImage* zItemImage;
};