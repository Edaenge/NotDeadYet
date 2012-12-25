/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/Gui/InventoryGui.h"
#include "GameFiles/Gui/CircularListGui.h"
#include <string>

class GuiManager
{
public:
	GuiManager();
	GuiManager(GraphicsEngine* ge);
	virtual ~GuiManager();
	void ToggleInventoryGui();
	void HideInventoryGui();
	void AddInventoryItemToGui(std::string itemIconPath);
	void ShowCircularItemGui();
	void HideCircularItemGui();
	void ShowLootingGui();
	void HideLootingGui();
	void ShowDeathGui();
	void HideDeathGui();
	void ToggleIngameMenu();
	bool IsGuiOpen();
	void Update(float deltaTime);
private:
	bool zLooting;
	bool zDeathGuiOpen;
	bool zInventoryOpen;
	bool zIngameMenuOpen;
	bool zCircularInventorySelectionOpen;
	
	float zLootingGuiShowTimer;
	float zInventoryGuiShowTimer;

	InventoryGui* zInvGui;
	CircularListGui* zInvCircGui;
	CircularListGui* zLootingGui;
	GraphicsEngine* zGraphicEngine;
};