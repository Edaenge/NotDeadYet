/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "InventoryGui.h"
#include "LootInventoryGui.h"
#include "CircularListGui.h"
#include "CraftingMenu.h"
#include <string>

struct Menu_select_data
{
	CIRCMENU zAction;
	Gui_Item_Data gid;
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
	void ToggleCraftingGui();
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

	bool IsLootingOpen() { return this->zLootOpen; }
	bool IsInventoryOpen() { return this->zInventoryOpen; }
	bool IsCraftOpen() { return this->zCraftOpen; }

	void Update(float deltaTime);
	Menu_select_data CheckCollisionInv();
	Menu_select_data CheckCrafting();
	void UpdateInventoryWeight(float weight);

	void ResetLoot();
	void ResetInventory();
	void ResetGui();
	void Resize(int width, int height);

	int GetLootingActor();
private:
	bool zLooting;
	bool zInventoryOpen;
	bool zLootOpen;
	bool zCraftOpen;
	bool zCircularInventorySelectionOpen;
	
	float zLootingGuiShowTimer;
	float zInventoryGuiShowTimer;

	Selected_Item_ReturnData zSir;
	int zSelectedCircMenu;
	bool zMinorFix;

	InventoryGui* zInvGui;
	LootInventoryGui* zLootGui;
	CircularListGui* zInvCircGui;
	CraftingMenu* zCraftingGui;
	//CircularListGui* zLootingGui;
	GraphicsEngine* zEng;
};