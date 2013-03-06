/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GuiElement.h"
#include <MaloW.h>

struct Selected_Item_ReturnData
{
	int ID;
	int type;
	int inventory;
};

struct Gui_Item_Data
{
	Gui_Item_Data()
	{
		this->zID = 0;
		this->zSlots = 0;

		this->zType = 0;
		this->zSubType = 0;

		this->zWeight = 0;
		this->zStacks = 0;
		this->zCanStack = false;

		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}
	Gui_Item_Data(const unsigned int ID, const unsigned int Type, const unsigned int subType, const unsigned int weight, 
		const unsigned int stack, const unsigned int Slots, const bool canStack, const std::string& name,const std::string& filePath, 
		const std::string& description)
	{
		this->zID = ID;
		this->zSlots = Slots;

		this->zType = Type;
		this->zSubType = subType;

		this->zWeight = weight;
		this->zStacks = stack;
		this->zCanStack = canStack;

		this->zName = name;
		this->zFilePath = filePath;
		this->zDescription = description;
	}
	unsigned int zID;
	unsigned int zSlots;
	unsigned int zType;
	unsigned int zSubType;

	unsigned int zWeight;
	unsigned int zStacks;
	bool zCanStack;

	std::string zName;
	std::string zFilePath;
	std::string zDescription;
};

class InventorySlotGui : public GuiElement
{
public:
	InventorySlotGui();
	InventorySlotGui(float x, float y, float width, float height, Gui_Item_Data gid);
	virtual ~InventorySlotGui();

	virtual bool AddToRenderer(GraphicsEngine* ge);
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	virtual void SetPosition(Vector2 pos);
	virtual void SetDimension(Vector2 dim);

	bool CheckCollision(float mouseX, float mouseY);
	
	void FadeOut(float value);
	void ShowGui();
	void HideGui();

	inline Gui_Item_Data GetGid(){ return this->zGid; }

	inline bool GetBlocked(){ return this->zBlocked; }
	inline void SetBlocked(bool value){ this->zBlocked = value; }

	inline bool GetEquipped(){ return this->zEquipped; }
	inline void SetEquipped(bool value){ this->zEquipped = value; }

	inline bool GetBlocker() { return this->zBlocker; }
	inline void SetBlocker(bool value){ this->zBlocker = value; }

	void SetStacks(int stacks) { this->zGid.zStacks = stacks; if(this->zStackText) this->zStackText->SetText(MaloW::convertNrToString((float)this->zGid.zStacks).c_str()); }

	void AddItemToSlot(Gui_Item_Data gid, bool invOpen, GraphicsEngine* ge);
	void RemoveItemFromSlot(bool invOpen, GraphicsEngine* ge);
;
private:
	iImage* zSlotImage;

	iText* zStackText;

	Gui_Item_Data zGid;

	bool zBlocked;
	bool zEquipped;
	bool zBlocker;
};