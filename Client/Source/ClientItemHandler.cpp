#include "Client.h"
#include "ClientServerMessages.h"

void Client::HandleWeaponUse(const unsigned int ID)
{
	Item* item = this->zPlayerInventory->GetPrimaryEquip();
	unsigned int itemStackID = 0;

	if(item->GetID() != ID)
	{
		MaloW::Debug("Item ID do not match in ClientItemHandler, HandleUseWeapon");
		return;
	}

	if (Projectile* proj = dynamic_cast<Projectile*>(item))
	{
		if(proj->GetItemSubType() == ITEM_SUB_TYPE_ROCK)
		{
			itemStackID = proj->GetID();
			unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
			Gui_Item_Data gid = Gui_Item_Data(proj->GetID(), totalWeight, 1, 
				proj->GetItemName(), proj->GetIconPath(), proj->GetItemDescription(), proj->GetItemType(), proj->GetItemSubType());
			if( proj->Use() )
			{
				this->zPlayerInventory->RemoveItemStack(proj->GetID(), 1);
				this->zGuiManager->RemoveInventoryItemFromGui(gid);
			}
		}
		else 
			return;
	}
	else if(RangedWeapon* ranged = dynamic_cast<RangedWeapon*>(item))
	{
		proj = this->zPlayerInventory->GetProjectile();
		
		if(!proj)
			return;

		itemStackID = proj->GetID();

		if( proj->Use() )
		{
			unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
			this->zPlayerInventory->RemoveItemStack(proj->GetID(), 1);
			Gui_Item_Data gid = Gui_Item_Data(proj->GetID(), totalWeight, 1, 
				proj->GetItemName(), proj->GetIconPath(), proj->GetItemDescription(), proj->GetItemType(), proj->GetItemSubType());
			this->zGuiManager->RemoveInventoryItemFromGui(gid);
		}
	}

}

void Client::HandleUseItem(const unsigned int ID)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ID);

	if (!item)
	{
		MaloW::Debug("Item cant be found");
		return;
	}

	if(item->GetItemType() == ITEM_TYPE_CONTAINER)
	{
		Container* container = dynamic_cast<Container*>(item);

		if (!container)
		{
			MaloW::Debug("dynamic cast Failed in Client::UseItem (Container)");
			return;
		}
		MaloW::Debug("Drinking");
		container->Use();
	}
	else if (item->GetItemType() == ITEM_TYPE_FOOD)
	{
		Food* food = dynamic_cast<Food*>(item);

		if (!food)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Food)");
			return;
		}

		if (!food->Use())
		{
			MaloW::Debug("Stack is Empty");
			return;
		}
		this->zPlayerInventory->RemoveItemStack(food->GetID(), 1);
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

		Gui_Item_Data gid = Gui_Item_Data(food->GetID(), totalWeight, 1, 
			food->GetItemName(), food->GetIconPath(), food->GetItemDescription(), food->GetItemType(), food->GetItemSubType());
		this->zGuiManager->RemoveInventoryItemFromGui(gid);

		MaloW::Debug("Eating");
	}
	else if (item->GetItemType() == ITEM_TYPE_MATERIAL)
	{

		Material* material = dynamic_cast<Material*>(item);

		if (!material)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Material)");
			return;
		}
		if (Messages::FileWrite())
			Messages::Debug("Crafting");

		if (!material->Use())
		{
			MaloW::Debug("Not Enough materials to Craft");
			return;
		}

		int removedStacks = material->GetRequiredStacksToCraft();

		this->zPlayerInventory->RemoveItemStack(material->GetID(), removedStacks);
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

		Gui_Item_Data gid = Gui_Item_Data(material->GetID(), totalWeight, removedStacks, 
			material->GetItemName(), material->GetIconPath(), material->GetItemDescription(), material->GetItemType(), material->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
	}
	else if (item->GetItemType() == ITEM_TYPE_BANDAGE)
	{
		Bandage* bandage = dynamic_cast<Bandage*>(item);

		if (!bandage)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Bandage)");
			return;
		}

		if (!bandage->Use())
		{
			MaloW::Debug("Stack is Empty");
			return;
		}

		this->zPlayerInventory->RemoveItemStack(bandage->GetID(), 1);

		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

		Gui_Item_Data gid = Gui_Item_Data(bandage->GetID(), totalWeight, 1, 
			bandage->GetItemName(), bandage->GetIconPath(), bandage->GetItemDescription(), bandage->GetItemType(), bandage->GetItemSubType());
		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		MaloW::Debug("Bandaging");
	}
}

//Todo Remove old projectile before adding new one(arrows)
void Client::HandleEquipItem(const unsigned int ItemID, const int Slot)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ItemID);
	bool success = false;

	if (!item)
	{
		MaloW::Debug("Item cant be found");
		return;
	}

	if (Slot == -1)
	{
		MaloW::Debug("Error In Client::EquipItem Item Slot is -1");
		return;
	}

	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
	{
		if (Slot != EQUIPMENT_SLOT_RANGED_WEAPON)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Weapon ID: " + MaloW::convertNrToString(EQUIPMENT_SLOT_RANGED_WEAPON) + "!= Slot: " + MaloW::convertNrToString((float)Slot));
			return;
		}
		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);
		if ( !rWpn )
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Bow)");
			return;
		}

		Item* prev = NULL;
		prev = this->zPlayerInventory->EquipRangedWeapon(rWpn, success);

		if(!success)
			return;
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
		if(prev)
		{
			

			Gui_Item_Data gid = Gui_Item_Data(prev->GetID(), totalWeight, 0, 
			prev->GetItemName(), prev->GetIconPath(), prev->GetItemDescription(), prev->GetItemType(), prev->GetItemSubType());

			this->zGuiManager->AddInventoryItemToGui(gid);
			this->zGuiManager->UnEquipItem(gid);
		}

		Gui_Item_Data gid = Gui_Item_Data(rWpn->GetID(), totalWeight, 0, 
			rWpn->GetItemName(), rWpn->GetIconPath(), rWpn->GetItemDescription(), rWpn->GetItemType(), rWpn->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(RANGED, gid);
		
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_PROJECTILE)
	{
		if (Slot != EQUIPMENT_SLOT_PROJECTILE)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Projectile: " + MaloW::convertNrToString(EQUIPMENT_SLOT_PROJECTILE) + " != Slot: " + MaloW::convertNrToString((float)Slot));
			return;
		}

		Projectile* projectile = dynamic_cast<Projectile*>(item);
		if (!projectile)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Projectile)");
			return;
		}
		
		int weigth = this->zPlayerInventory->GetTotalWeight();
		Item* prev = NULL;
		prev = this->zPlayerInventory->EquipProjectile(projectile);
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
		if(prev)
		{
			//If weigth has increased
			if(weigth < this->zPlayerInventory->GetTotalWeight())
			{
				//Delete projectile
				Item* temp = this->zPlayerInventory->RemoveItem(projectile);
				SAFE_DELETE(temp);
				projectile = this->zPlayerInventory->GetProjectile();
			}
			else
			{
				

				Gui_Item_Data gid = Gui_Item_Data(prev->GetID(), totalWeight, prev->GetStackSize(), 
					prev->GetItemName(), prev->GetIconPath(), prev->GetItemDescription(), prev->GetItemType(), prev->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(gid);
			}
		}

		Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), totalWeight, projectile->GetStackSize(), 
			projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(PROJECTILE, gid);
		
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
	{
		if (Slot != EQUIPMENT_SLOT_MELEE_WEAPON)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Weapon: " + MaloW::convertNrToString(EQUIPMENT_SLOT_MELEE_WEAPON) + " != Slot: " + MaloW::convertNrToString((float)Slot));
			return;
		}

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);
		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Knife/Axe)");
			return;
		}

		Item* prev = NULL;
		prev = this->zPlayerInventory->EquipMeleeWeapon(mWpn, success);

		if(!success)
			return;
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
		if(prev)
		{
			

			Gui_Item_Data gid = Gui_Item_Data(prev->GetID(), totalWeight, 0, 
				prev->GetItemName(), prev->GetIconPath(), prev->GetItemDescription(), prev->GetItemType(), prev->GetItemSubType());

			this->zGuiManager->AddInventoryItemToGui(gid);
			this->zGuiManager->UnEquipItem(gid);
		}

		Gui_Item_Data gid = Gui_Item_Data(mWpn->GetID(), totalWeight, 0, 
			mWpn->GetItemName(), mWpn->GetIconPath(), mWpn->GetItemDescription(), mWpn->GetItemType(), mWpn->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(MELEE, gid);
		

		return;
	}
}

bool Client::HandleUnEquipItem(const unsigned int ItemID, const int Slot)
{
	if (Slot == EQUIPMENT_SLOT_PROJECTILE)
	{
		Projectile* projectile = this->zPlayerInventory->GetProjectile();

		if (projectile)
		{
			if (projectile->GetID() == ItemID)
			{
				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), totalWeight, projectile->GetStackSize(), 
					projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(gid);

				this->zPlayerInventory->UnEquipProjectile();

				return true;
			}
			MaloW::Debug("Item With ID doesn't exist in Ammo ID: " + MaloW::convertNrToString((float)ItemID));
			return false;
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	if (Slot == EQUIPMENT_SLOT_RANGED_WEAPON)
	{
		Weapon* wpn = this->zPlayerInventory->GetRangedWeapon();

		if (wpn)
		{
			if (wpn->GetID() == ItemID)
			{
				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(wpn->GetID(), totalWeight, 0, 
					wpn->GetItemName(), wpn->GetIconPath(), wpn->GetItemDescription(), wpn->GetItemType(), wpn->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(gid);

				this->zPlayerInventory->UnEquipRangedWeapon();
				
				return true;
			}
			MaloW::Debug("Item With ID doesn't exist in Weapon ID: " + MaloW::convertNrToString((float)ItemID));
			return false;
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	if (Slot == EQUIPMENT_SLOT_MELEE_WEAPON)
	{
		Weapon* wpn = this->zPlayerInventory->GetMeleeWeapon();

		if (wpn)
		{
			if (wpn->GetID() == ItemID)
			{
				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(wpn->GetID(), totalWeight, 0, 
					wpn->GetItemName(), wpn->GetIconPath(), wpn->GetItemDescription(), wpn->GetItemType(), wpn->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(gid);

				this->zPlayerInventory->UnEquipMeleeWeapon();
				
				return true;
			}
			MaloW::Debug("Item With ID doesn't exist in Weapon ID: " + MaloW::convertNrToString((float)ItemID));
			return false;
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	/*if (Slot == EQUIPMENT_SLOT_HEAD)
	{
		Gear* head = this->zPlayerInventory->GetGear(EQUIPMENT_SLOT_HEAD);

		if (head)
		{
			if (head->GetID() == ItemID)
			{
				this->zPlayerInventory->UnEquipGear(EQUIPMENT_SLOT_HEAD);

				if(this->zPlayerInventory->AddItem(head))
				{
					Gui_Item_Data gid = Gui_Item_Data(head->GetID(), head->GetWeight(), 0, 
						head->GetItemName(), head->GetIconPath(), head->GetItemDescription(), head->GetItemType(), head->GetItemSubType());

					this->zGuiManager->AddInventoryItemToGui(gid);

					return true;
				}
				return false;
			}
			MaloW::Debug("Item With ID doesn't exist in Head Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	if (Slot == EQUIPMENT_SLOT_CHEST)
	{
		Gear* chest = this->zPlayerInventory->GetGear(EQUIPMENT_SLOT_CHEST);

		if (chest)
		{
			if (chest->GetID() == ItemID)
			{
				this->zPlayerInventory->UnEquipGear(EQUIPMENT_SLOT_CHEST);

				if(this->zPlayerInventory->AddItem(chest))
				{
					Gui_Item_Data gid = Gui_Item_Data(chest->GetID(), chest->GetWeight(), 0, chest->GetItemName(),
						chest->GetIconPath(), chest->GetItemDescription(), chest->GetItemType());

					this->zGuiManager->AddInventoryItemToGui(gid);

					return true;
				}
				return false;
			}
			MaloW::Debug("Item With ID doesn't exist in Chest Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	if (Slot == EQUIPMENT_SLOT_LEGS)
	{
		Gear* legs = this->zPlayerInventory->GetGear(EQUIPMENT_SLOT_LEGS);

		if (legs)
		{
			if (legs->GetID() == ItemID)
			{
				this->zPlayerInventory->UnEquipGear(EQUIPMENT_SLOT_LEGS);

				if(this->zPlayerInventory->AddItem(legs))
				{
					Gui_Item_Data gid = Gui_Item_Data(legs->GetID(), legs->GetWeight(), 0, legs->GetItemName(),
						legs->GetIconPath(), legs->GetItemDescription(), legs->GetItemType());

					this->zGuiManager->AddInventoryItemToGui(gid);

					return true;
				}
				return false;;
			}
			MaloW::Debug("Item With ID doesn't exist in Legs Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}

	if (Slot == EQUIPMENT_SLOT_BOOTS)
	{
		Gear* boots = this->zPlayerInventory->GetGear(EQUIPMENT_SLOT_BOOTS);

		if (boots)
		{
			if (boots->GetID() == ItemID)
			{
				this->zPlayerInventory->UnEquipGear(EQUIPMENT_SLOT_BOOTS);

				if(this->zPlayerInventory->AddItem(boots))
				{
					Gui_Item_Data gid = Gui_Item_Data(boots->GetID(), boots->GetWeight(), 0, boots->GetItemName(),
						boots->GetIconPath(), boots->GetItemDescription(), boots->GetItemType());

					this->zGuiManager->AddInventoryItemToGui(gid);

					return true;
				}
				return false;;
			}
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
		return false;
	}*/
	return false;
}

void Client::HandleRemoveEquipment(const unsigned int ItemID, const int Slot)
{
	if (Slot == EQUIPMENT_SLOT_RANGED_WEAPON)
	{
		Weapon* weapon = this->zPlayerInventory->GetRangedWeapon();

		if (weapon)
		{
			if (ItemID == weapon->GetID())
			{
				if (Messages::FileWrite())
					Messages::Debug("Weapon UnEquipped " + weapon->GetItemName() + " ID: " + MaloW::convertNrToString((float)weapon->GetID()));

				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(weapon->GetID(), totalWeight, weapon->GetStackSize(), 
					weapon->GetItemName(), weapon->GetIconPath(), weapon->GetItemDescription(), weapon->GetItemType(), weapon->GetItemSubType());

				this->zGuiManager->UnEquipItem(gid);

				Item* temp = this->zPlayerInventory->RemoveItem(ItemID);
				SAFE_DELETE(temp);

				this->zPlayerInventory->UnEquipRangedWeapon();
			}
		}
		return;
	}

	if (Slot == EQUIPMENT_SLOT_MELEE_WEAPON)
	{
		Weapon* weapon = this->zPlayerInventory->GetMeleeWeapon();

		if (weapon)
		{
			if (ItemID == weapon->GetID())
			{
				if (Messages::FileWrite())
					Messages::Debug("Weapon UnEquipped " + weapon->GetItemName() + " ID: " + MaloW::convertNrToString((float)weapon->GetID()));

				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(weapon->GetID(), totalWeight, weapon->GetStackSize(), 
					weapon->GetItemName(), weapon->GetIconPath(), weapon->GetItemDescription(), weapon->GetItemType(), weapon->GetItemSubType());

				this->zGuiManager->UnEquipItem(gid);

				Item* temp = this->zPlayerInventory->RemoveItem(ItemID);
				SAFE_DELETE(temp);

				this->zPlayerInventory->UnEquipMeleeWeapon();
			}
		}
		return;
	}

	if (Slot == EQUIPMENT_SLOT_PROJECTILE)
	{
		Projectile* projectile = this->zPlayerInventory->GetProjectile();

		if (projectile)
		{
			if (ItemID == projectile->GetID())
			{
				if (Messages::FileWrite())
					Messages::Debug("Ammo UnEquipped " + projectile->GetItemName() + " ID: " + MaloW::convertNrToString((float)projectile->GetID()));

				unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

				Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), totalWeight, projectile->GetStackSize(), 
					projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());
				
				Item* temp = this->zPlayerInventory->RemoveItem(projectile);
				SAFE_DELETE(temp);


				this->zGuiManager->UnEquipItem(gid);

				this->zPlayerInventory->RemoveItem(projectile);
									this->zPlayerInventory->UnEquipProjectile();
			}
		}
		return;
	}
}

void Client::SendUnEquipItem(const unsigned int ID, const int Slot)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)ID);

	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->zServerChannel->Send(msg);
}

void Client::SendEquipItem( const unsigned int ID )
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIP_ITEM, (float)ID);
	this->zServerChannel->Send(msg);
}

void Client::SendLootItemMessage(const unsigned int ID, const unsigned int ItemID, const int Type, const int SubType)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_LOOT_ITEM, (float)ID);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_ID, (float)ItemID);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)Type);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_SUB_TYPE, (float)SubType);
	this->zServerChannel->Send(msg);
}

void Client::SendCraftItemMessage( const unsigned int ID )
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_CRAFT, (float)ID);
	this->zServerChannel->Send(msg);
}

void Client::SendUseItemMessage(const unsigned int ID)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
	this->zServerChannel->Send(msg);
}

void Client::SendDropItemMessage(const unsigned int ID)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_DROP_ITEM, (float)ID);

	this->zServerChannel->Send(msg);
}

void Client::HandleRemoveInventoryItem(const unsigned int ID)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ID);
	if (item)
	{
		int stackSize = item->GetStackSize();
		item = this->zPlayerInventory->RemoveItem(item);

		if(item)
		{
			unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();

			Gui_Item_Data gid = Gui_Item_Data(item->GetID(), totalWeight, stackSize, 
				item->GetItemName(), item->GetIconPath(), item->GetItemDescription(), item->GetItemType(), item->GetItemSubType());

			this->zGuiManager->RemoveInventoryItemFromGui(gid);
			if (Messages::FileWrite())
				Messages::Debug("Item Removed on Client");

			delete item, item = NULL;
		}

	}
	
}

void Client::HandleAddInventoryItem(const std::vector<std::string>& msgArray)
{
	std::string itemName = "Unknown";
	std::string itemDescription = "<UNKNOWN DESCRIPTION>";
	std::string itemIconFilePath = "none";
	int itemWeight = 0;
	int itemStackSize = 0;
	unsigned int ID = 0;
	unsigned int itemType = 10;
	unsigned int itemSubType = 10;
	float weaponDamage = 0.0f;
	float weaponRange = 0.0f;
	float projectileDamage = 0.0f;
	float projectileVelocity = 0.0f;
	int craftingType = -1;
	int stacksRequired = 10000;
	float hunger = 0.0f;
	int maxUse = 0;
	int currUse = 0;
	char key[256];
	for (auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			itemDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			itemIconFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			itemWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
		}
		else if(strcmp(key, M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			itemStackSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
		}
		else if(strcmp(key, M_ITEM_ID.c_str()) == 0)
		{
			ID = this->zMsgHandler.ConvertStringToInt(M_ITEM_ID, (*it));
		}
		else if(strcmp(key, M_ITEM_SUB_TYPE.c_str()) == 0)
		{
			itemSubType = this->zMsgHandler.ConvertStringToInt(M_ITEM_SUB_TYPE, (*it));
		}
		else if(strcmp(key, M_WEAPON_DAMAGE.c_str()) == 0)
		{
			weaponDamage = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_DAMAGE, (*it));
		}
		else if(strcmp(key, M_WEAPON_RANGE.c_str()) == 0)
		{
			weaponRange = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_RANGE, (*it));
		}
		else if(strcmp(key, M_PROJECTILE_VELOCITY.c_str()) == 0)
		{
			projectileVelocity = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_VELOCITY, (*it));
		}
		else if(strcmp(key, M_PROJECTILE_DAMAGE.c_str()) == 0)
		{
			projectileDamage = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_DAMAGE, (*it));
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
		}
		else if(strcmp(key, M_CONTAINER_MAX.c_str()) == 0)
		{
			maxUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_MAX, (*it));
		}
		else if(strcmp(key, M_CONTAINER_CURRENT.c_str()) == 0)
		{
			currUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_CURRENT, (*it));
		}
		else if(strcmp(key, M_MATERIAL_CRAFTING_TYPE.c_str()) == 0)
		{
			craftingType = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_CRAFTING_TYPE, (*it));
		}
		else if(strcmp(key, M_MATERIAL_STACKS_REQUIRED.c_str()) == 0)
		{
			stacksRequired = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_STACKS_REQUIRED, (*it));
		}
	}
	if (itemType == -1)
	{
		MaloW::Debug("Wrong or no Item Type sent from server in Client::HandleAddInventoryItem ItemType: " + MaloW::convertNrToString((float)itemType));
		return;
	}
	//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
	Item* item = NULL;
	switch (itemType)
	{
	case ITEM_TYPE_FOOD:
		item = new Food(ID, itemType, itemSubType, hunger);
		item->SetStacking(true);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED:
		item = new RangedWeapon(ID, itemType, itemSubType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE:
		item = new MeleeWeapon(ID, itemType, itemSubType, weaponDamage, weaponRange);
		item->SetStacking(false);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_CONTAINER:
		item = new Container(ID, itemType, itemSubType, maxUse, currUse);
		item->SetStacking(false);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_PROJECTILE:
		item = new Projectile(ID, itemType, itemSubType, projectileVelocity, projectileDamage);
		item->SetStacking(true);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL:
		item = new Material(ID, itemType, itemSubType, craftingType, stacksRequired);
		item->SetStacking(true);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR:
		item = new Gear(ID, itemSubType, itemType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_BANDAGE:
		item = new Bandage(ID, itemType, itemSubType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	default:
		MaloW::Debug("Items wasn't found in the switch case type: " + MaloW::convertNrToString((float)itemType));
		break;
	}

	if (!item->GetStacking())
	{
		itemStackSize = 0;
	}
	bool stacked = false;
	
	if (this->zPlayerInventory->AddItem(item, stacked))
	{
		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
		Projectile* projectile = this->zPlayerInventory->GetProjectile();

		if(stacked && projectile && (projectile->GetItemSubType() == itemSubType))
		{
			

			Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), totalWeight, projectile->GetStackSize(), 
				projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());

			this->zGuiManager->RemoveInventoryItemFromGui(gid);
			this->zGuiManager->EquipItem(PROJECTILE, gid);
		}
		else
		{
			Gui_Item_Data gid = Gui_Item_Data(ID, totalWeight, itemStackSize, itemName, itemIconFilePath, itemDescription, itemType, itemSubType);
			this->zGuiManager->AddInventoryItemToGui(gid);
		}
	}
	else
	{
		SAFE_DELETE(item);
	}
}