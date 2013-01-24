#include "GameFiles/ClientSide/GuiManager.h"

static const std::string DEATH_GUI_PATH						= "Media/Use_v02.png";
static const std::string LOOTING_GUI_PATH					= "Media/Use_v02.png";
static const std::string INVENTORY_GUI_PATH					= "Media/Inventory_v02.png";
static const std::string IN_GAME_MENU_GUI_PATH				= "Media/Use_v02.png";
static const std::string INVENTORY_ITEM_SELECTION_GUI_PATH	= "Media/Use_v02.png";

static const float GUI_DISPLAY_TIMER					= 2.0f;

GuiManager::GuiManager()
{
	this->zLooting = false;
	this->zDeathGuiOpen = false;
	this->zInventoryOpen = false;
	this->zIngameMenuOpen = false;
	this->zCircularInventorySelectionOpen = false;

	this->zLootingGuiShowTimer = 0.0f;
	this->zInventoryGuiShowTimer = 0.0f;
	
	this->zInvGui = NULL;
	//this->zLootingGui = NULL;
	this->zInvCircGui = NULL;
	this->zGraphicEngine = NULL;

	zSelectedItem = 0;
	zSelectedCircMenu = 0;
	zMinorFix = false;
}

GuiManager::GuiManager(GraphicsEngine* ge)
{
	this->zLooting = false;
	this->zDeathGuiOpen = false;
	this->zInventoryOpen = false;
	this->zIngameMenuOpen = false;
	this->zCircularInventorySelectionOpen = false;

	this->zGraphicEngine = ge;

	this->zLootingGuiShowTimer = 0.0f;
	this->zInventoryGuiShowTimer = 0.0f;

	float windowWidth = (float)(ge->GetEngineParameters()->windowWidth);
	float windowHeight = (float)(ge->GetEngineParameters()->windowHeight);

	float x = windowWidth * 0.25f;
	float y = windowHeight * 0.25f;
	float width = (500.0f / 1024.0f) * windowWidth;
	float height = (500.0f / 768.0f) * windowHeight;
	
	this->zInvGui = new InventoryGui(windowWidth - width, windowHeight - height, width, height, INVENTORY_GUI_PATH);

	Vector2 mousePosition = this->zGraphicEngine->GetKeyListener()->GetMousePosition();

	x = mousePosition.x;
	y = mousePosition.y;
	width = (200.0f / 1024.0f) * windowWidth;
	height = (200.0f / 768.0f) * windowHeight;
	this->zInvCircGui = new CircularListGui(x, y, width, height, INVENTORY_ITEM_SELECTION_GUI_PATH);

	//this->zLootingGui = new CircularListGui(x, y, width, height, LOOTING_GUI_PATH);
	zSelectedItem = 0;
	zSelectedCircMenu = 0;
	zMinorFix = false;
}

GuiManager::~GuiManager()
{
	if (this->zInvGui)
	{
		delete this->zInvGui;
		this->zInvGui = NULL;
	}

	if (this->zInvCircGui)
	{
		delete this->zInvCircGui;
		this->zInvCircGui = NULL;
	}

	//if (this->zLootingGui)
	//{
	//	delete this->zLootingGui;
	//	this->zLootingGui = NULL;
	//}
}

void GuiManager::HideInventoryGui()
{
	if (this->zInventoryOpen)
	{
		//Hide Inventory
		this->zInventoryOpen = false;
		//this->zInvGui->RemoveFromRenderer(this->zGraphicEngine);
	}
}

void GuiManager::ToggleInventoryGui()
{
	if (!this->zInventoryOpen)
	{
		//Show Inventory
		this->zInvGui->AddToRenderer(this->zGraphicEngine);
		this->zInventoryOpen = true;
		this->zInventoryGuiShowTimer = GUI_DISPLAY_TIMER;
	}
	else
	{
		//Hide Inventory
		this->zInventoryOpen = false;
		this->zInvGui->RemoveFromRenderer(this->zGraphicEngine);
		if(this->zCircularInventorySelectionOpen)
		{
			this->zCircularInventorySelectionOpen = false;
			this->zInvCircGui->RemoveFromRenderer(this->zGraphicEngine);
		}
	}
}

void GuiManager::AddInventoryItemToGui(const Gui_Item_Data gid)
{
	this->zInvGui->AddItemToGui(gid, this->zInventoryOpen, this->zGraphicEngine);
}

void GuiManager::RemoveInventoryItemFromGui(const int ID, int stacks)
{
	this->zInvGui->RemoveItemFromGui(ID, stacks);
}

void GuiManager::ShowCircularItemGui()
{
	if (this->zInventoryOpen)
	{
		//Check Collision
		if (!this->zCircularInventorySelectionOpen)
		{
			//Set Gui Position to Mouse Position
			Vector2 mousePosition = this->zGraphicEngine->GetKeyListener()->GetMousePosition();
			Vector2 dimension = this->zInvCircGui->GetDimension();

			float x = mousePosition.x - dimension.x * 0.5f;
			float y = mousePosition.y - dimension.y * 0.5f;
			this->zInvCircGui->SetPosition(x, y);

			//Show Gui
			this->zCircularInventorySelectionOpen = true;
			this->zInvCircGui->AddToRenderer(this->zGraphicEngine);
		}
	}
}

void GuiManager::HideCircularItemGui()
{
	//Hide Gui
	if (this->zCircularInventorySelectionOpen)
	{
		this->zCircularInventorySelectionOpen = false;
		this->zInvCircGui->RemoveFromRenderer(this->zGraphicEngine);
	}
}

void GuiManager::ShowLootingGui(std::vector<Gui_Item_Data> gui_Item_Data)
{
	if(!this->zLooting)
	{
		//Set Gui Position to Mouse Position
		//Vector2 mousePosition = this->zGraphicEngine->GetKeyListener()->GetMousePosition();
		//Vector2 dimension = this->zLootingGui->GetDimension();
		//float x = mousePosition.x - dimension.x * 0.5f;
		//float y = mousePosition.y - dimension.y * 0.5f;
		//this->zLootingGui->SetPosition(x, y);

		//Show Loot Gui
		this->zLooting = true;
		//this->zLootingGui->AddToRenderer(this->zGraphicEngine);
		this->zLootingGuiShowTimer = GUI_DISPLAY_TIMER;
	}
}

void GuiManager::HideLootingGui()
{
	if(this->zLooting)
	{
		//Hide Loot Gui
		this->zLooting = false;
		
		//this->zLootingGui->RemoveFromRenderer(this->zGraphicEngine);
	}
}

void GuiManager::ToggleIngameMenu()
{
	if (!this->zIngameMenuOpen)
	{
		this->zIngameMenuOpen = true;
	}
	else
	{
		this->zIngameMenuOpen = false;
	}
}

void GuiManager::ShowDeathGui()
{
	if (!this->zDeathGuiOpen)
	{
		this->zDeathGuiOpen = true;
	}
}

void GuiManager::HideDeathGui()
{
	if (this->zDeathGuiOpen)
	{
		this->zDeathGuiOpen = false;
	}
}

void GuiManager::Update(float deltaTime)
{
	if (!this->zInventoryOpen)
	{
		if (this->zInventoryGuiShowTimer > 0.0f)
		{
			this->zInventoryGuiShowTimer -= deltaTime;
			this->zInvGui->FadeOut(deltaTime);
		}
		else
		{
			this->HideInventoryGui();
		}
	}
	//
	//if (!this->zLooting)
	//{
	//	if (this->zLootingGuiShowTimer > 0.0f)
	//	{
	//		this->zLootingGuiShowTimer -= deltaTime;
	//	}
	//	else
	//	{
	//		this->HideLootingGui();
	//	}
	//}
}

bool GuiManager::IsGuiOpen()
{
	return (this->zInventoryOpen || this->zLooting || this->zDeathGuiOpen || this->zIngameMenuOpen);
}

Menu_select_data GuiManager::CheckCollisionInv()
{
	if(!zGraphicEngine->GetKeyListener()->IsClicked(2) && zMinorFix)
	{
		zMinorFix = false;
	}

	if(this->zCircularInventorySelectionOpen)
	{
		Vector2 mousePos = zGraphicEngine->GetKeyListener()->GetMousePosition();
		this->zSelectedCircMenu = this->zInvCircGui->CheckCollision(mousePos.x, mousePos.y, zGraphicEngine->GetKeyListener()->IsClicked(1), zGraphicEngine);

		if(this->zSelectedCircMenu >= 0 && this->zSelectedCircMenu < 4)
		{
			this->HideCircularItemGui();
			Menu_select_data msd;
			msd.zAction = (CIRCMENU)this->zSelectedCircMenu;
			msd.zID = this->zSelectedItem;
			Menu_select_data returnVal;
			return msd;
		}
		else if(!zGraphicEngine->GetKeyListener()->IsClicked(2))
		{
			zMinorFix = false;
			this->HideCircularItemGui();
		}

	}
	else if( this->zInventoryOpen )
	{
		Vector2 mousePos = zGraphicEngine->GetKeyListener()->GetMousePosition();
		this->zSelectedItem = this->zInvGui->CheckCollision(mousePos.x, mousePos.y, zGraphicEngine->GetKeyListener()->IsClicked(2), zGraphicEngine);
		if(zSelectedItem != -1 && !this->zCircularInventorySelectionOpen && !zMinorFix)
		{
			zMinorFix = true;
			this->ShowCircularItemGui();
		}
		
	}
	Menu_select_data msd;
	msd.zAction = (CIRCMENU)-1;
	msd.zID = -1;
	return msd;
}