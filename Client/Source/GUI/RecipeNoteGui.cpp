#include "RecipeNoteGui.h"
#include "Material.h"
#include "MaloW.h"
#include "Safe.h"

RecipeNoteGui::RecipeNoteGui(Vector2 pos, const std::map<unsigned int, unsigned int>* mats) : 
	GuiElement(pos.x, pos.y, TEXTHEIGHT * mats->size(), TEXTHEIGHT * mats->size(), RECIPENOTETEXTURE)
{
	float windowHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;

	this->zNrOfTexts = 0;
	// Saves all the mats in text strings
	for(auto it = mats->cbegin(); it != mats->cend(); it++)
	{
		switch (it->first)
		{
		case ITEM_SUB_TYPE_SMALL_STICK:
			this->zMatsText[zNrOfTexts++] = "SMALL STICKS: " + MaloW::convertNrToString(it->second);
			break;
		case ITEM_SUB_TYPE_MEDIUM_STICK:
			this->zMatsText[zNrOfTexts++] = "MEDIUM STICKS: " + MaloW::convertNrToString(it->second);
			break;
		case ITEM_SUB_TYPE_LARGE_STICK:
			this->zMatsText[zNrOfTexts++] = "LARGE STICKS: " + MaloW::convertNrToString(it->second);
			break;
		case ITEM_SUB_TYPE_THREAD:
			this->zMatsText[zNrOfTexts++] = "THREAD: " + MaloW::convertNrToString(it->second);
			break;
		case ITEM_SUB_TYPE_DISENFECTANT_LEAF:
			this->zMatsText[zNrOfTexts++] = "DISENFECTANT LEAF: " + MaloW::convertNrToString(it->second);
			break;
		default:
			this->zMatsText[zNrOfTexts++] = "ERROR";
			break;
		}
	}
	// Calc positions.
	this->SetDimension(Vector2((300.0f / 1024.0f) * dx, 
		(TEXTSTARTY + (this->zNrOfTexts * TEXTHEIGHT) + TEXTSTARTY / 768.0f) * windowHeight));

	GuiElement::AddToRenderer(GetGraphics());
	int lCurrentText = 0;
	for(int i = 0; i < this->zNrOfTexts; i++)
	{
		Vector2 pos = Vector2(this->zX + TEXTSTARTX, this->zY + TEXTSTARTY + (this->zNrOfTexts * TEXTHEIGHT));
		this->zRenderTextes[lCurrentText] = GetGraphics()->CreateText(this->zMatsText[lCurrentText].c_str(), pos, 2, "Media/fonts/new");
		lCurrentText++;
	}
	

}

RecipeNoteGui::~RecipeNoteGui()
{
	for(int i = 0; i < MAXNROFMATS; i++)
	{
		if(this->zRenderTextes[i])
			SAFE_DELETE(this->zRenderTextes[i]);
	}
}

bool RecipeNoteGui::AddToRenderer( GraphicsEngine* ge )
{
	GuiElement::AddToRenderer(ge);
	return true;
}

bool RecipeNoteGui::RemoveFromRenderer( GraphicsEngine* ge )
{
	GuiElement::RemoveFromRenderer(ge);
	return true;
}

