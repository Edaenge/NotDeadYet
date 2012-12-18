/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Gui/GuiElement.h"

class CircularListGui : public GuiElement
{
public:
	CircularListGui();
	CircularListGui(float x, float y, float width, float height, std::string textureName);
	virtual ~CircularListGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	bool CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge);
private:
	bool zPressed;
	bool zHovered;
};