/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "InventoryGui.h"
#include "CircularListGui.h"
#include <string>

struct Menu_select_data
{
	CIRCMENU zAction;
	int zID;
	int zType;
};
class GuiManager
{
public:
	GuiManager();
	GuiManager(GraphicsEngine* ge);
	virtual ~GuiManager();
	void ToggleInventoryGui();
	void HideInventoryGui();
	void AddInventoryItemToGui(const Gui_Item_Data gid);
	void RemoveInventoryItemFromGui(const int ID, int stacks);
	void EquipItem(int type, const Gui_Item_Data gid);
	void UnEquipItem(const int ID, int stacks);
	void ShowCircularItemGui();
	void HideCircularItemGui();
	void ShowLootingGui(std::vector<Gui_Item_Data> gui_Item_Data);
	void HideLootingGui();
	void ShowDeathGui();
	void HideDeathGui();
	void ToggleIngameMenu();
	bool IsGuiOpen();
	void Update(float deltaTime);
	Menu_select_data CheckCollisionInv();

	void Resize(int width, int height);
private:
	bool zLooting;
	bool zDeathGuiOpen;
	bool zInventoryOpen;
	bool zIngameMenuOpen;
	bool zCircularInventorySelectionOpen;
	
	float zLootingGuiShowTimer;
	float zInventoryGuiShowTimer;

	int zSelectedItem;
	int zSelectedType;
	int zSelectedCircMenu;
	bool zMinorFix;

	InventoryGui* zInvGui;
	CircularListGui* zInvCircGui;
	//CircularListGui* zLootingGui;
	GraphicsEngine* zEng;
};