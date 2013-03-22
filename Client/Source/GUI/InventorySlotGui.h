/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GuiElement.h"
#include <MaloW.h>

struct Gui_Item_Data
{
	Gui_Item_Data()
	{
		this->zID = -1;
		this->zSlots = -1;

		this->zType = -1;
		this->zSubType = -1;

		this->zWeight = -1;
		this->zStacks = 0;
		this->zCanStack = false;

		this->zName = "Unknown";
		this->zFilePath = "Unknown";
		this->zDescription = "Unknown";
	}

	Gui_Item_Data(const unsigned int& ID, 
		const unsigned int& Type, 
		const unsigned int& subType, 
		const float& weight, 
		const unsigned int stack, 
		const unsigned int& Slots, 
		const bool& canStack, 
		const std::string& name, 
		const std::string& filePath, 
		const std::string& description) :
		zID(ID),
		zSlots(Slots),
		zType(Type),
		zSubType(subType),
		zWeight(weight),
		zStacks(stack),
		zCanStack(canStack),
		zName(name),
		zFilePath(filePath),
		zDescription(description)
	{
	}

	int zID;
	int zSlots;
	int zType;
	int zSubType;

	float zWeight;
	unsigned int zStacks;
	bool zCanStack;

	std::string zName;
	std::string zFilePath;
	std::string zDescription;
};

struct Selected_Item_ReturnData
{
	Gui_Item_Data gid;
	int inventory;
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

	inline const Gui_Item_Data& GetGid() const { return this->zGid; }

	inline const bool& GetBlocked() const { return this->zBlocked; }
	inline void SetBlocked(const bool& value) { this->zBlocked = value; }

	inline const bool& GetEquipped() const { return this->zEquipped; }
	inline void SetEquipped(const bool& value) { this->zEquipped = value; }

	inline const bool& GetBlocker() const { return this->zBlocker; }
	inline void SetBlocker(const bool& value) { this->zBlocker = value; }

	void SetStacks(const int& stacks) 
	{ 
		this->zGid.zStacks = stacks; 

		if(this->zStackText)
		{
			this->zStackText->SetText(MaloW::convertNrToString((float)this->zGid.zStacks).c_str()); 
		}
	}

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