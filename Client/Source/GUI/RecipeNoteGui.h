#pragma once

#include "GuiElement.h"
#include <Vector.h>
#include <map>

#define MAXNROFMATS 5
#define RECIPENOTETEXTURE ""
#define TEXTHEIGHT 35.0f
#define TEXTSTARTX 10.0f
#define TEXTSTARTY 10.0f

class RecipeNoteGui : public GuiElement
{
private:
	int zNrOfTexts;

	iText* zRenderTextes[MAXNROFMATS];
	std::string zMatsText[MAXNROFMATS];
public:
	RecipeNoteGui(Vector2 pos, const std::map<unsigned int, unsigned int>* mats);
	virtual ~RecipeNoteGui();

	//Inherited from GuiElement.
	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

};