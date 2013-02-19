#include "GuiManager.h"

static const std::string DEATH_GUI_PATH						= "Media/Icons/Use_v02.png";
static const std::string LOOTING_GUI_PATH					= "Media/Icons/Use_v02.png";
static const std::string INVENTORY_GUI_PATH					= "Media/Icons/Inventory_Full.png";
static const std::string IN_GAME_MENU_GUI_PATH				= "Media/Icons/Use_v02.png";
static const std::string INVENTORY_ITEM_SELECTION_GUI_PATH	= "Media/Icons/Menu_Circle.png";

static const float GUI_DISPLAY_TIMER					= 2.0f;

static const float InvXPos = 538.0f;
static const float InvYPos = 108.0f;
static const float InvWidth = 486.0f;
static const float InvHeight = 660.0f;

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
	this->zEng = NULL;

	zSelectedItem = -1;
	zSelectedType = -1;
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

	this->zEng = ge;

	this->zLootingGuiShowTimer = 0.0f;
	this->zInventoryGuiShowTimer = 0.0f;

	float windowWidth = (float)(ge->GetEngineParameters().WindowWidth);
	float windowHeight = (float)(ge->GetEngineParameters().WindowHeight);

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	float x = offSet + (InvXPos / 1024.0f) * dx;
	float y = (InvYPos / 768.0f) * windowHeight;
	float width = (InvWidth / 1024.0f) * dx;
	float height = (InvHeight / 768.0f) * windowHeight;
	
	this->zInvGui = new InventoryGui(windowWidth - width, windowHeight - height, width, height, INVENTORY_GUI_PATH);

	Vector2 mousePosition = this->zEng->GetKeyListener()->GetMousePosition();

	x = mousePosition.x;
	y = mousePosition.y;
	width = (CIRCLISTRADIUS / 1024.0f) * dx;
	height = (CIRCLISTRADIUS / 768.0f) * windowHeight;
	int options[4] = {UNEQUIP, NOTHING, NOTHING, USE};
	this->zInvCircGui = new CircularListGui(x, y, width, height, INVENTORY_ITEM_SELECTION_GUI_PATH, options);

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
		this->zInvGui->AddToRenderer(this->zEng);
		this->zInventoryOpen = true;
		this->zInventoryGuiShowTimer = GUI_DISPLAY_TIMER;
	}
	else
	{
		//Hide Inventory
		this->zInventoryOpen = false;
		this->zInvGui->RemoveFromRenderer(this->zEng);
		if(this->zCircularInventorySelectionOpen)
		{
			this->zCircularInventorySelectionOpen = false;
			this->zInvCircGui->RemoveFromRenderer(this->zEng);
		}
	}
}

void GuiManager::AddInventoryItemToGui(const Gui_Item_Data gid)
{
	this->zInvGui->AddItemToGui(gid, this->zInventoryOpen, this->zEng);
}

void GuiManager::RemoveInventoryItemFromGui(const Gui_Item_Data gid)
{
	this->zInvGui->RemoveItemFromGui(gid, this->zInventoryOpen, this->zEng);
}

void GuiManager::ShowCircularItemGui()
{
	if (this->zInventoryOpen)
	{
		//Check Collision
		if (!this->zCircularInventorySelectionOpen)
		{
			//Set Gui Position to Mouse Position
			Vector2 mousePosition = this->zEng->GetKeyListener()->GetMousePosition();
			Vector2 dimension = this->zInvCircGui->GetDimension();

			float x = mousePosition.x - dimension.x * 0.5f;
			float y = mousePosition.y - dimension.y * 0.5f;
			this->zInvCircGui->SetPosition(Vector2(x, y));

			//Show Gui
			this->zCircularInventorySelectionOpen = true;
			this->zInvCircGui->AddToRenderer(this->zEng);
		}
	}
}

void GuiManager::HideCircularItemGui()
{
	//Hide Gui
	if (this->zCircularInventorySelectionOpen)
	{
		this->zCircularInventorySelectionOpen = false;
		this->zInvCircGui->RemoveFromRenderer(this->zEng);
	}
}

void GuiManager::ShowLootingGui(std::vector<Looting_Gui_Data> looting_gui_Data)
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
	if(!zEng->GetKeyListener()->IsClicked(2) && zMinorFix)
	{
		zMinorFix = false;
	}

	if(this->zCircularInventorySelectionOpen)
	{
		Vector2 mousePos = zEng->GetKeyListener()->GetMousePosition();
		this->zSelectedCircMenu = this->zInvCircGui->CheckCollision(mousePos.x, mousePos.y, zEng->GetKeyListener()->IsClicked(1), zEng);

		if(this->zSelectedCircMenu != -1)
		{
			this->HideCircularItemGui();
			Menu_select_data msd;
			msd.zAction = (CIRCMENU)this->zSelectedCircMenu;
			msd.zID = this->zSelectedItem;
			msd.zType = this->zSelectedType;
			return msd;
		}
		else if(!zEng->GetKeyListener()->IsClicked(2))
		{
			zMinorFix = false;
			this->HideCircularItemGui();
		}

	}
	else if( this->zInventoryOpen )
	{
		Vector2 mousePos = zEng->GetKeyListener()->GetMousePosition();
		Selected_Item_ReturnData sir;
		sir = this->zInvGui->CheckCollision(mousePos.x, mousePos.y, zEng->GetKeyListener()->IsClicked(2), zEng);
		zSelectedItem = sir.ID;
		zSelectedType = sir.type;
		this->zInvCircGui->Adjust(sir.type, sir.inventory);
		if(zSelectedItem != -1 && !this->zCircularInventorySelectionOpen && !zMinorFix)
		{
			zMinorFix = true;
			this->ShowCircularItemGui();
		}
		
	}
	Menu_select_data msd;
	msd.zAction = (CIRCMENU)-1;
	msd.zID = -1;
	msd.zType = -1;
	return msd;
}

void GuiManager::EquipItem( int type, const Gui_Item_Data gid )
{
	this->zInvGui->EquipItem(type, gid, this->zInventoryOpen);
}

void GuiManager::UnEquipItem( const Gui_Item_Data gid )
{
	this->zInvGui->UnEquipItem(gid, this->zInventoryOpen, this->zEng);
}

void GuiManager::Resize( int width, int height )
{
	float dx = ((float)height * 4.0f) / 3.0f;

	float guiWidth = (InvWidth / 1024.0f) * dx;
	float guiHeight = (InvHeight / 768.0f) * height;

	this->zInvGui->SetPosition(width - guiWidth, height - guiHeight);
	this->zInvGui->SetDimension(Vector2(guiWidth, guiHeight));

	this->zInvGui->Resize(width, height, dx);
	this->zInvCircGui->Resize(width, height, dx);
}
