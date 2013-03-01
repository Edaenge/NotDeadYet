/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "InventoryGui.h"
#include "LootInventoryGui.h"
#include "CircularListGui.h"
#include <string>

struct Menu_select_data
{
	CIRCMENU zAction;
	int zID;
	int zType;
};

struct Looting_Gui_Data
{
	Looting_Gui_Data()
	{
		zGui_Data = Gui_Item_Data();
		zActorID = 0;
	}
	Gui_Item_Data zGui_Data;
	int zActorID;
};
class GuiManager
{
public:
	GuiManager();
	GuiManager(GraphicsEngine* ge);
	virtual ~GuiManager();
	void ToggleInventoryGui();
	void ToggleLootGui(unsigned int lootActorId);
	void AddInventoryItemToGui(const Gui_Item_Data gid);
	void AddLootItemToLootGui(const Gui_Item_Data gid);
	void RemoveLootItemFromLootGui(const Gui_Item_Data gid);
	void RemoveInventoryItemFromGui(const Gui_Item_Data gid);
	void EquipItem(const Gui_Item_Data gid);
	void UnEquipItem( const Gui_Item_Data gid );
	void ShowCircularItemGui();
	void HideCircularItemGui();
	void ShowLootingGui(std::vector<Looting_Gui_Data> gui_Item_Data);
	bool IsLootingOpen() {return this->zLootOpen;}
	bool IsInventoryOpen() {return this->zInventoryOpen;}
	void Update(float deltaTime);
	Menu_select_data CheckCollisionInv();
	void UpdateInventoryWeight(float weight);

	void Resize(int width, int height);
private:
	bool zLooting;
	bool zInventoryOpen;
	bool zLootOpen;
	bool zCircularInventorySelectionOpen;
	
	float zLootingGuiShowTimer;
	float zInventoryGuiShowTimer;

	int zSelectedItem;
	int zSelectedType;
	int zSelectedCircMenu;
	bool zMinorFix;

	InventoryGui* zInvGui;
	LootInventoryGui* zLootGui;
	CircularListGui* zInvCircGui;
	//CircularListGui* zLootingGui;
	GraphicsEngine* zEng;
};