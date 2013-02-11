#pragma once

#include "Graphics.h"
#include "Element.h"

class GUIPicture : public Element
{
private:

public:
	GUIPicture();
	GUIPicture(float x, float y, float z, string textureName, float width, float height);
	virtual ~GUIPicture();

	/*! Adds the element to the renderer*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the element from the renderer*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	void Resize(int oldWindowWidth, int oldWindowHeight, int windowWidth, int windowHeight);

};