#include "GuiManager.h"

#define DEATH_GUI_PATH "Media/Icons/Use_v02.png"
#define LOOTING_GUI_PATH "Media/Icons/Use_v02.png"
#define INVENTORY_GUI_PATH "Media/InGameUI/Inventory_BG.png"
#define LOOT_GUI_PATH "Media/InGameUI/Loot_BG.png"
#define IN_GAME_MENU_GUI_PATH "Media/Icons/Use_v02.png"
#define INVENTORY_ITEM_SELECTION_GUI_PATH "Media/Icons/Menu_Circle.png"

static const float GUI_DISPLAY_TIMER					= 2.0f;

#define INVXPOS 538.0f
#define INVYPOS 108.0f
#define INVWIDTH 486.0f
#define INVHEIGHT 660.0f
#define LOOTHEIGHT 540.0f

GuiManager::GuiManager()
{
	this->zLooting = false;
	this->zInventoryOpen = false;
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
	this->zInventoryOpen = false;
	this->zLootOpen = false;
	this->zCircularInventorySelectionOpen = false;

	this->zEng = ge;

	this->zLootingGuiShowTimer = 0.0f;
	this->zInventoryGuiShowTimer = 0.0f;

	float windowWidth = (float)(ge->GetEngineParameters().WindowWidth);
	float windowHeight = (float)(ge->GetEngineParameters().WindowHeight);

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	float x = offSet + (INVXPOS / 1024.0f) * dx;
	float y = (INVYPOS / 768.0f) * windowHeight;
	float width = (INVWIDTH / 1024.0f) * dx;
	float height = (INVHEIGHT / 768.0f) * windowHeight;
	float heightLoot = (LOOTHEIGHT / 768.0f) * windowHeight;

	this->zInvGui = new InventoryGui(windowWidth - width, windowHeight - height, width, height, INVENTORY_GUI_PATH);

	this->zLootGui = new LootInventoryGui(0, windowHeight - heightLoot, width, heightLoot, LOOT_GUI_PATH);

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

	if (this->zLootGui)
	{
		delete this->zLootGui;
		this->zLootGui = NULL;
	}
	//if (this->zLootingGui)
	//{
	//	delete this->zLootingGui;
	//	this->zLootingGui = NULL;
	//}
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

void GuiManager::ToggleLootGui(unsigned int lootActorId)
{
	if (!this->zLootOpen)
	{
		this->zLootGui->SetLootActor(lootActorId);
		//Show Inventory
		this->zLootGui->AddToRenderer(this->zEng);
		this->zLootOpen = true;
	}
	else
	{
		this->zLootGui->SetLootActor(0);
		//Hide Inventory
		this->zLootOpen = false;
		this->zLootGui->RemoveFromRenderer(this->zEng);
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

void GuiManager::AddLootItemToLootGui(const Gui_Item_Data gid)
{
	this->zLootGui->AddItemToGui(gid, this->zLootOpen, this->zEng);
}

void GuiManager::RemoveLootItemFromLootGui(const Gui_Item_Data gid)
{
	this->zLootGui->RemoveItemFromGui(gid, this->zLootOpen, this->zEng);
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

			float windowWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
			float windowHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;

			bool changeYPos = false;
			bool changeXPos = false;

			if(x  > windowWidth - dimension.x)
			{
				x = windowWidth - dimension.x;
				changeXPos = true;
			}
			else if(x < 0)
			{
				x = 0;
				changeXPos = true;
			}

			if(y > windowHeight - dimension.y)
			{
				y = windowHeight - dimension.y;
				changeYPos = true;
			}
			else if(y < 0)
			{
				y = 0;
				changeYPos = true;
			}

			if(changeXPos || changeYPos)
			{
				GetGraphics()->GetKeyListener()->SetMousePosition(Vector2(x + (dimension.x * 0.5f), y + (dimension.y * 0.5f)));
			}
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

Menu_select_data GuiManager::CheckCollisionInv()
{
	if(!zEng->GetKeyListener()->IsClicked(2) && zMinorFix)
	{
		zMinorFix = false;
	}

	if(this->zCircularInventorySelectionOpen)
	{
		Vector2 mousePos = zEng->GetKeyListener()->GetMousePosition();
		this->zSelectedCircMenu = this->zInvCircGui->CheckCollision(mousePos.x, mousePos.y, (zEng->GetKeyListener()->IsClicked(1) || !zEng->GetKeyListener()->IsClicked(2)), zEng);

		if(this->zSelectedCircMenu != -1 || !zEng->GetKeyListener()->IsClicked(2))
		{
			this->HideCircularItemGui();
			Menu_select_data msd;
			msd.zAction = (CIRCMENU)this->zSelectedCircMenu;
			msd.zID = this->zSelectedItem;
			msd.zType = this->zSelectedType;
			return msd;
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
	if( zSelectedItem == -1 && this->zLootOpen)
	{
		Vector2 mousePos = zEng->GetKeyListener()->GetMousePosition();
		Selected_Item_ReturnData sir;
		sir = this->zLootGui->CheckCollision(mousePos.x, mousePos.y, zEng->GetKeyListener()->IsClicked(2), zEng);
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

void GuiManager::EquipItem( const Gui_Item_Data gid )
{
	this->zInvGui->EquipItem(gid, this->zInventoryOpen);
}

void GuiManager::UnEquipItem( const Gui_Item_Data gid )
{
	this->zInvGui->UnEquipItem(gid, this->zInventoryOpen, this->zEng);
}

void GuiManager::Resize( int width, int height )
{
	float dx = ((float)height * 4.0f) / 3.0f;

	float guiWidth = (INVWIDTH / 1024.0f) * dx;
	float guiHeight = (INVHEIGHT / 768.0f) * height;

	this->zInvGui->SetPosition(width - guiWidth, height - guiHeight);
	this->zInvGui->SetDimension(Vector2(guiWidth, guiHeight));

	this->zInvGui->Resize(width, height, dx);
	this->zInvCircGui->Resize(width, height, dx);
}
void GuiManager::UpdateInventoryWeight( float weight )
{
	this->zInvGui->UpdateInventoryWeight(weight);
}

void GuiManager::ResetLoot()
{
	this->zLootGui->Reset(this->zLootOpen);
}

void GuiManager::ResetInventory()
{
	this->zInvGui->Reset(this->zInventoryOpen);
}

void GuiManager::ResetGui()
{
	this->ResetLoot();
	this->ResetInventory();
}

int GuiManager::GetLootingActor()
{
	if(this->zLootGui)
	{
		return this->zLootGui->GetLootActor();
	}
	return 0;
}
