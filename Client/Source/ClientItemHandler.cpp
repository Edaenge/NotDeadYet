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
			proj->SetStackSize(proj->GetStackSize()-1);
			itemStackID = proj->GetID();
		}
		else 
			return;
	}
	else if(RangedWeapon* ranged = dynamic_cast<RangedWeapon*>(item))
	{
		proj = this->zPlayerInventory->GetProjectile();

		if(!proj)
			return;

		proj->SetStackSize(proj->GetStackSize()-1);
		itemStackID = proj->GetID();
	}

	this->zGuiManager->RemoveInventoryItemFromGui(itemStackID, 1);
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

		int oldStacks = food->GetStackSize();
		if (!food->Use())
		{
			MaloW::Debug("Stack is Empty");
			return;
		}
		int newStacks = food->GetStackSize();

		int stacks = oldStacks - newStacks;
		this->zPlayerInventory->RemoveItemStack(food->GetID(), stacks);

		this->zGuiManager->RemoveInventoryItemFromGui(food->GetID(), stacks);
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

		int oldStacks = material->GetStackSize();
		if (!material->Use())
		{
			MaloW::Debug("Not Enough materials to Craft");
			return;
		}
		int newStacks = material->GetStackSize();

		int stacks = oldStacks - newStacks;
		this->zPlayerInventory->RemoveItemStack(material->GetID(), stacks);

		this->zGuiManager->RemoveInventoryItemFromGui(material->GetID(), stacks);
	}
	else if (item->GetItemType() == ITEM_TYPE_BANDAGE)
	{
		Bandage* bandage = dynamic_cast<Bandage*>(item);

		if (!bandage)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Bandage)");
			return;
		}

		int oldStacks = bandage->GetStackSize();
		if (!bandage->Use())
		{
			MaloW::Debug("Stack is Empty");
			return;
		}
		
		int newStacks = bandage->GetStackSize();

		int stacks = oldStacks - newStacks;
		this->zPlayerInventory->RemoveItemStack(bandage->GetID(), stacks);

		this->zGuiManager->RemoveInventoryItemFromGui(bandage->GetID(), stacks);
		MaloW::Debug("Bandaging");
	}
}

void Client::HandleEquipItem(const unsigned int ItemID, const int Slot)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ItemID);

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
		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Bow)");
			return;
		}

		this->zPlayerInventory->EquipRangedWeapon(rWpn);

		Gui_Item_Data gid = Gui_Item_Data(rWpn->GetID(), rWpn->GetWeight(), 0, 
			rWpn->GetItemName(), rWpn->GetIconPath(), rWpn->GetItemDescription(), rWpn->GetItemType(), rWpn->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(rWpn->GetID(), rWpn->GetStackSize());
		this->zGuiManager->EquipItem(RANGED, gid);
		
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_PROJECTILE)
	{
		if (Slot != EQUIPMENT_SLOT_PROJECTILE)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not an Ammo: " + MaloW::convertNrToString(EQUIPMENT_SLOT_PROJECTILE) + " != Slot: " + MaloW::convertNrToString((float)Slot));
			return;
		}

		Projectile* projectile = dynamic_cast<Projectile*>(item);
		if (!projectile)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Arrow)");
			return;
		}
		
		Projectile* oldProjectile = this->zPlayerInventory->GetProjectile();
		if (oldProjectile)
		{
			if (oldProjectile->GetItemType() != projectile->GetItemType())
			{
				if(this->zPlayerInventory->AddItem(oldProjectile))
				{
					Gui_Item_Data gid = Gui_Item_Data(oldProjectile->GetID(), oldProjectile->GetWeight(), oldProjectile->GetStackSize(), 
						oldProjectile->GetItemName(), oldProjectile->GetIconPath(), oldProjectile->GetItemDescription(), oldProjectile->GetItemType(), oldProjectile->GetItemSubType());

					this->zGuiManager->AddInventoryItemToGui(gid);

					this->zPlayerInventory->UnEquipProjectile();
				}
				else
				{
					return;
				}
			}
			else
			{
				int stacks = oldProjectile->GetStackSize() + projectile->GetStackSize();
				projectile->SetStackSize(stacks);
			}
		}
		this->zPlayerInventory->EquipProjectile(projectile);

		Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), projectile->GetWeight(), projectile->GetStackSize(), 
			projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(projectile->GetID(), projectile->GetStackSize());
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

		this->zPlayerInventory->EquipMeleeWeapon(mWpn);

		Gui_Item_Data gid = Gui_Item_Data(mWpn->GetID(), mWpn->GetWeight(), 0, 
			mWpn->GetItemName(), mWpn->GetIconPath(), mWpn->GetItemDescription(), mWpn->GetItemType(), mWpn->GetItemSubType());

		this->zGuiManager->RemoveInventoryItemFromGui(mWpn->GetID(), mWpn->GetStackSize());
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

				Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), projectile->GetWeight(), projectile->GetStackSize(), 
					projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription(), projectile->GetItemType(), projectile->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(projectile->GetID(), projectile->GetStackSize());

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
				Gui_Item_Data gid = Gui_Item_Data(wpn->GetID(), wpn->GetWeight(), 0, 
					wpn->GetItemName(), wpn->GetIconPath(), wpn->GetItemDescription(), wpn->GetItemType(), wpn->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(wpn->GetID(), 0);

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
				Gui_Item_Data gid = Gui_Item_Data(wpn->GetID(), wpn->GetWeight(), 0, 
					wpn->GetItemName(), wpn->GetIconPath(), wpn->GetItemDescription(), wpn->GetItemType(), wpn->GetItemSubType());

				this->zGuiManager->AddInventoryItemToGui(gid);
				this->zGuiManager->UnEquipItem(wpn->GetID(), 0);

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

				Gui_Item_Data gid = Gui_Item_Data(weapon->GetID(), weapon->GetWeight(), weapon->GetStackSize(), 
					weapon->GetItemName(), weapon->GetIconPath(), weapon->GetItemDescription(), weapon->GetItemType(), weapon->GetItemSubType());

				this->zGuiManager->UnEquipItem(weapon->GetID(), 1);

				this->zPlayerInventory->RemoveItem(ItemID);

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

				Gui_Item_Data gid = Gui_Item_Data(weapon->GetID(), weapon->GetWeight(), weapon->GetStackSize(), 
					weapon->GetItemName(), weapon->GetIconPath(), weapon->GetItemDescription(), weapon->GetItemType(), weapon->GetItemSubType());

				this->zGuiManager->UnEquipItem(weapon->GetID(), 1);

				this->zPlayerInventory->RemoveItem(ItemID);

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

				this->zGuiManager->UnEquipItem(projectile->GetID(), projectile->GetStackSize());

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
		if(this->zPlayerInventory->RemoveItem(item))
		{
			this->zGuiManager->RemoveInventoryItemFromGui(ID, stackSize);
			if (Messages::FileWrite())
				Messages::Debug("Item Removed on Client");
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
	if (this->zPlayerInventory->AddItem(item))
	{
		if (!item->GetStacking())
		{
			itemStackSize = 0;
		}
		Gui_Item_Data gid = Gui_Item_Data(ID, itemWeight, itemStackSize, itemName, itemIconFilePath, itemDescription, itemType, itemSubType);
		this->zGuiManager->AddInventoryItemToGui(gid);
	}
	else
	{
		SAFE_DELETE(item);
	}
}