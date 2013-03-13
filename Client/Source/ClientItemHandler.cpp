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
			
			if( proj->Use() )
			{
				this->zPlayerInventory->RemoveItemStack(proj->GetID(), 1);
				Gui_Item_Data gid = this->MakeGID(proj);
				gid.zStacks = 1;
				this->zGuiManager->RemoveInventoryItemFromGui(gid);
				this->zGuiManager->UpdateInventoryWeight( (float)this->zPlayerInventory->GetTotalWeight());
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
			this->zPlayerInventory->RemoveItemStack(proj->GetID(), 1);
			Gui_Item_Data gid = this->MakeGID(proj);
			gid.zStacks = 1;
			this->zGuiManager->RemoveInventoryItemFromGui(gid);
			this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
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
		AudioManager* am = AudioManager::GetInstance();
		IEventHandle* temp;
		Actor* tActor = this->zActorManager->GetActor(this->zID);
		int FMOD_RESULT_TEMP;
		if(tActor->GetModel().find("female"))
			FMOD_RESULT_TEMP = am->GetEventHandle(EVENTID_NOTDEADYET_WOMAN_DRINKWATER, temp);
		else
			FMOD_RESULT_TEMP = am->GetEventHandle(EVENTID_NOTDEADYET_MAN_DRINKWATER, temp);

		if(FMOD_RESULT_TEMP == FMOD_OK)
		{
			temp->Play();
		}
		if (!container)
		{
			MaloW::Debug("dynamic cast Failed in Client::UseItem (Container)");
			return;
		}
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

		Gui_Item_Data gid = this->MakeGID(food);
		gid.zStacks = 1; // Stacks to be removed
		this->zGuiManager->RemoveInventoryItemFromGui(gid);

		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
	}
	/*else if (item->GetItemType() == ITEM_TYPE_MATERIAL)
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

		Gui_Item_Data gid = this->MakeGID(material);
		gid.zStacks = removedStacks; // Stacks to be removed
		this->zGuiManager->RemoveInventoryItemFromGui(gid);

		unsigned int totalWeight = this->zPlayerInventory->GetTotalWeight();
		this->zGuiManager->UpdateInventoryWeight(totalWeight);
	}*/
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

		Gui_Item_Data gid = this->MakeGID(bandage);
		gid.zStacks = 1; // Stacks to be removed
		this->zGuiManager->RemoveInventoryItemFromGui(gid);

		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
	}
}

void Client::HandleCraftItem(const unsigned int ID, const unsigned int Stacks)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ID);

	if (item)
	{
		item->DecreaseStackSize(Stacks);
		this->zPlayerInventory->RemoveItemStack(ID, Stacks);

		Gui_Item_Data gid = this->MakeGID(item);
		gid.zStacks = Stacks;

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
	}
	else
	{
		 MaloW::Debug("Failed to find item in inventory");
	}
}

void Client::HandleFillItem(const unsigned int ID, const int currentUses)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ID);

	if (!item)
	{
		MaloW::Debug("Failed to find item in Client::HandleFillItem");
		return;
	}

	if (Container* container = dynamic_cast<Container*>(item))
	{
		Gui_Item_Data gid = this->MakeGID(container);

		this->zGuiManager->RemoveInventoryItemFromGui(gid);

		container->SetRemainingUses(currentUses);

		this->zGuiManager->AddInventoryItemToGui(gid);
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

		if(prev)
		{
			

			Gui_Item_Data gid = this->MakeGID(prev);

			this->zGuiManager->AddInventoryItemToGui(gid);
			this->zGuiManager->UnEquipItem(gid);
		}

		Gui_Item_Data gid = this->MakeGID(rWpn);

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(gid);
		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
		
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
				Gui_Item_Data gid = MakeGID(prev);

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(gid);
			}
		}

		Gui_Item_Data gid = MakeGID(projectile);

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(gid);

		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
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
		if(prev)
		{
			Gui_Item_Data gid = this->MakeGID(prev);

			this->zGuiManager->AddInventoryItemToGui(gid);
			this->zGuiManager->UnEquipItem(gid);
		}

		Gui_Item_Data gid = this->MakeGID(mWpn);

		this->zGuiManager->RemoveInventoryItemFromGui(gid);
		this->zGuiManager->EquipItem(gid);
		this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
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

				Gui_Item_Data gid = this->MakeGID(projectile);
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
				Gui_Item_Data gid = this->MakeGID(wpn);

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
				Gui_Item_Data gid = this->MakeGID(wpn);

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

				Item* temp = this->zPlayerInventory->RemoveItem(ItemID);

				Gui_Item_Data gid = this->MakeGID(weapon);
				this->zGuiManager->UnEquipItem(gid);

				this->zPlayerInventory->UnEquipRangedWeapon();
				this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
				SAFE_DELETE(temp);
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

				Item* temp = this->zPlayerInventory->RemoveItem(ItemID);


				Gui_Item_Data gid = this->MakeGID(weapon);
				this->zGuiManager->UnEquipItem(gid);

				SAFE_DELETE(temp);

				this->zPlayerInventory->UnEquipMeleeWeapon();
				this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
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

				Item* temp = this->zPlayerInventory->RemoveItem(projectile);


				Gui_Item_Data gid = this->MakeGID(projectile);
				this->zGuiManager->UnEquipItem(gid);

				SAFE_DELETE(temp);

				this->zPlayerInventory->UnEquipProjectile();
				this->zGuiManager->UpdateInventoryWeight((float)this->zPlayerInventory->GetTotalWeight());
			}
		}
		return;
	}
}

void Client::SendUnEquipItem( const unsigned int ID )
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)ID);

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
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_OBJECT_ID, (float)ItemID);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)Type);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_SUB_TYPE, (float)SubType);
	this->zServerChannel->Send(msg);
}

void Client::SendCraftItemMessage(const int Type, const int SubType)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_CRAFT, 0);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)Type);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_SUB_TYPE, (float)SubType);
	this->zServerChannel->Send(msg);
}

void Client::SendItemFill(const unsigned int ID)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_FILL, (float)ID);
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
		item = this->zPlayerInventory->RemoveItem(item);

		if(item)
		{
			Gui_Item_Data gid = this->MakeGID(item);

			this->zGuiManager->RemoveInventoryItemFromGui(gid);
			if (Messages::FileWrite())
				Messages::Debug("Item Removed on Client");

			delete item, item = NULL;
		}
	}
	this->zGuiManager->UpdateInventoryWeight((float)zPlayerInventory->GetTotalWeight());
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
	unsigned int itemSlotSize = 1000;
	float projectileDamage = 0.0f;
	float projectileVelocity = 0.0f;
	float hunger = 0.0f;
	int maxUse = 0;
	int currUse = 0;
	char key[256];
	
	auto it_string_end = msgArray.end();
	for (auto it = msgArray.begin() + 1; it < it_string_end; it++)
	{
		sscanf_s((*it).c_str(), "%s ", key, sizeof(key));

		if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it), true);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			itemDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it), true);
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
		else if(strcmp(key, M_ITEM_SLOT_SIZE.c_str()) == 0)
		{
			itemSlotSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_SLOT_SIZE, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
		}
		else if(strcmp(key, M_OBJECT_ID.c_str()) == 0)
		{
			ID = this->zMsgHandler.ConvertStringToInt(M_OBJECT_ID, (*it));
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
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED:
		item = new RangedWeapon(ID, itemType, itemSubType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE:
		item = new MeleeWeapon(ID, itemType, itemSubType, weaponDamage, weaponRange);
		item->SetStacking(false);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_CONTAINER:
		item = new Container(ID, itemType, itemSubType, maxUse, currUse);
		item->SetStacking(false);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_PROJECTILE:
		item = new Projectile(ID, itemType, itemSubType, projectileVelocity, projectileDamage);
		item->SetStacking(true);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL:
		item = new Material(ID, itemType, itemSubType);
		item->SetStacking(true);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR:
		item = new Gear(ID, itemSubType, itemType);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_BANDAGE:
		item = new Bandage(ID, itemType, itemSubType);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MISC:
		item = new Misc(ID, itemType, itemSubType);
		item->SetItemName(itemName);
		item->SetSlotSize(itemSlotSize);
		item->SetItemWeight(itemWeight);
		item->SetStackSize(itemStackSize);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	default:
		MaloW::Debug("Items wasn't found in the switch case type: " + MaloW::convertNrToString((float)itemType));
		return;
		break;
	}

	if (!item->GetStacking())
	{
		itemStackSize = 0;
	}
	bool stacked = false;
	
	Gui_Item_Data unstacked_Gid = this->MakeGID(item);
	if (this->zPlayerInventory->AddItem(item, stacked))
	{
		if(stacked)
		{
			Item* existingItem = zPlayerInventory->SearchAndGetItemFromType(item->GetItemType(), item->GetItemSubType());

			Gui_Item_Data gid = this->MakeGID(existingItem);

			Projectile* projItem = dynamic_cast<Projectile*>(existingItem);
			if( projItem != NULL && projItem == zPlayerInventory->GetProjectile() )
			{
				this->zGuiManager->RemoveInventoryItemFromGui(gid);
				this->zGuiManager->EquipItem(gid);
				this->zGuiManager->RemoveLootItemFromLootGui(gid);
			}
			else
			{
				this->zGuiManager->AddInventoryItemToGui(unstacked_Gid);
				this->zGuiManager->RemoveLootItemFromLootGui(unstacked_Gid);
			}

			if(item->GetStackSize() == 0)
				SAFE_DELETE(item);
		}
		else
		{
			Gui_Item_Data gid = this->MakeGID(item);
			this->zGuiManager->AddInventoryItemToGui(gid);
			this->zGuiManager->RemoveLootItemFromLootGui(gid);
		}
	}
	else
	{
		SAFE_DELETE(item);
	}
	this->zGuiManager->UpdateInventoryWeight((float)zPlayerInventory->GetTotalWeight());
}

Gui_Item_Data Client::MakeGID( Item* item )
{
	Gui_Item_Data gid = Gui_Item_Data(item->GetID(), item->GetItemType(), item->GetItemSubType() , item->GetWeight(),
		item->GetStackSize(), item->GetSlotSize(), item->GetStacking(), item->GetItemName(), item->GetIconPath(), item->GetItemDescription());

	return gid;
}