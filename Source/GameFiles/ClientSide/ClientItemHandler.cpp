#include "Client.h"
#include "ClientServerMessages.h"

void Client::HandleUseItem(const int ID)
{
	Item* item = this->zPlayerInventory->SearchAndGetItem(ID);

	if (!item)
	{
		MaloW::Debug("Item cant be found");
		return;
	}

	if(item->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN)
	{
		Container* container = dynamic_cast<Container*>(item);

		if (!container)
		{
			MaloW::Debug("dynamic cast Failed in Client::UseItem (Container)");
			return;
		}
		container->Use();

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || item->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
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

		return;
	}
}

void Client::HandleEquipItem(const int ItemID, const int Slot)
{
	Item* item = this->zPlayerInventory->EquipItem(ItemID);

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

	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		if (Slot != EQUIPMENT_SLOT_WEAPON)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Weapon ID: " + MaloW::convertNrToString(EQUIPMENT_SLOT_WEAPON) + "!= Slot: " + MaloW::convertNrToString(Slot));
			return;
		}
		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);
		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Bow)");
			return;
		}
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
		Equipment* eq = player->GetEquipmentPtr();

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			this->zPlayerInventory->AddItem(oldWeapon);
		}

		eq->EquipWeapon(rWpn);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		if (Slot != EQUIPMENT_SLOT_WEAPON)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Weapon ID: " + MaloW::convertNrToString(EQUIPMENT_SLOT_WEAPON) + "!= Slot: " + MaloW::convertNrToString(Slot));
			return;
		}

		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);
		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Rock)");
			return;
		}
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
		Equipment* eq = player->GetEquipmentPtr();

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			this->zPlayerInventory->AddItem(oldWeapon);
		}

		eq->EquipWeapon(rWpn);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
	{
		if (Slot != EQUIPMENT_SLOT_AMMO)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not an Ammo ID: " + MaloW::convertNrToString(EQUIPMENT_SLOT_AMMO) + "!= Slot: " + MaloW::convertNrToString(Slot));
			return;
		}

		Projectile* projectile = dynamic_cast<Projectile*>(item);
		if (!projectile)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Arrow)");
			return;
		}
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
		Equipment* eq = player->GetEquipmentPtr();

		Projectile* oldProjectile = eq->GetProjectile();
		if (oldProjectile)
		{
			this->zPlayerInventory->AddItem(oldProjectile);
		}
		eq->EquipProjectile(projectile);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE || item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE)
	{
		if (Slot != EQUIPMENT_SLOT_WEAPON)
		{
			MaloW::Debug("Error In Client::EquipItem Item Slot Is Not a Weapon ID: " + MaloW::convertNrToString(EQUIPMENT_SLOT_WEAPON) + "!= Slot: " + MaloW::convertNrToString(Slot));
			return;
		}

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);
		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Client::EquipItem (Knife/Axe)");
			return;
		}

		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
		Equipment* eq = player->GetEquipmentPtr();

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			this->zPlayerInventory->AddItem(oldWeapon);
		}

		eq->EquipWeapon(mWpn);

		return;
	}
}

void Client::HandleUnEquipItem(const int ItemID, const int Slot)
{
	PlayerObject* pObject = this->zObjectManager->SearchAndGetPlayerObject(this->zID);

	if (!pObject)
	{
		MaloW::Debug("Client: this Player cant be found");
		return;
	}

	Equipment* eq = pObject->GetEquipmentPtr();

	if (Slot == EQUIPMENT_SLOT_AMMO)
	{
		Projectile* projectile = eq->GetProjectile();

		if (projectile)
		{
			if (projectile->GetID() == ItemID)
			{
				eq->UnEquipProjectile();

				this->zPlayerInventory->AddItem(projectile);
				Gui_Item_Data gid = Gui_Item_Data(projectile->GetID(), projectile->GetItemName(), projectile->GetIconPath(), projectile->GetItemDescription());
				this->zGuiManager->AddInventoryItemToGui(gid);
				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Ammo ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_WEAPON)
	{
		Weapon* wpn = eq->GetWeapon();

		if (wpn)
		{
			if (wpn->GetID() == ItemID)
			{
				eq->UnEquipWeapon();

				this->zPlayerInventory->AddItem(wpn);

				Gui_Item_Data gid = Gui_Item_Data(wpn->GetID(), wpn->GetItemName(),
					wpn->GetIconPath(), wpn->GetItemDescription());

				this->zGuiManager->AddInventoryItemToGui(gid);
				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Weapon ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_HEAD)
	{
		Gear* head = eq->GetGear(EQUIPMENT_SLOT_HEAD);

		if (head)
		{
			if (head->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_HEAD);

				this->zPlayerInventory->AddItem(head);

				Gui_Item_Data gid = Gui_Item_Data(head->GetID(), head->GetItemName(),
					head->GetIconPath(), head->GetItemDescription());

				this->zGuiManager->AddInventoryItemToGui(gid);
				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Head Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_CHEST)
	{
		Gear* chest = eq->GetGear(EQUIPMENT_SLOT_CHEST);

		if (chest)
		{
			if (chest->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_CHEST);

				this->zPlayerInventory->AddItem(chest);

				Gui_Item_Data gid = Gui_Item_Data(chest->GetID(), chest->GetItemName(),
					chest->GetIconPath(), chest->GetItemDescription());

				this->zGuiManager->AddInventoryItemToGui(gid);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Chest Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_LEGS)
	{
		Gear* legs = eq->GetGear(EQUIPMENT_SLOT_LEGS);

		if (legs)
		{
			if (legs->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_LEGS);

				this->zPlayerInventory->AddItem(legs);

				Gui_Item_Data gid = Gui_Item_Data(legs->GetID(), legs->GetItemName(),
					legs->GetIconPath(), legs->GetItemDescription());

				this->zGuiManager->AddInventoryItemToGui(gid);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Legs Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_BOOTS)
	{
		Gear* boots = eq->GetGear(EQUIPMENT_SLOT_BOOTS);

		if (boots)
		{
			if (boots->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_BOOTS);

				this->zPlayerInventory->AddItem(boots);

				Gui_Item_Data gid = Gui_Item_Data(boots->GetID(), boots->GetItemName(),
					boots->GetIconPath(), boots->GetItemDescription());

				this->zGuiManager->AddInventoryItemToGui(gid);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}
}

void Client::HandleRemoveEquipment(const int ItemID, const int Slot)
{
	if (Slot == EQUIPMENT_SLOT_WEAPON)
	{
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);

		Equipment* eq = player->GetEquipmentPtr();

		Weapon* weapon = eq->GetWeapon();

		if (weapon)
		{
			delete weapon;
			weapon = NULL;
		}
		eq->UnEquipWeapon();

		return;
	}

	if (Slot == EQUIPMENT_SLOT_AMMO)
	{
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);

		Equipment* eq = player->GetEquipmentPtr();

		Projectile* projectile = eq->GetProjectile();

		if (projectile)
		{
			delete projectile;
			projectile = NULL;
		}

		eq->UnEquipProjectile();

		return;
	}
}

void Client::SendUnEquipItem(const int ID, const int Slot)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, ID);

	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, Slot);

	this->zServerChannel->sendData(msg);
}

void Client::SendPickupItemMessage(const int ID)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PICKUP_ITEM, ID);

	this->zServerChannel->sendData(msg);
}

void Client::SendDropItemMessage(const int ID)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_DROP_ITEM, ID);

	this->zServerChannel->sendData(msg);
}

void Client::HandleRemoveInventoryItem(const int ID)
{
	int index = this->zPlayerInventory->Search(ID);
	if(this->zPlayerInventory->RemoveItem(index))
	{
		this->zGuiManager->RemoveInventoryItemFromGui(ID);
		if (Messages::FileWrite())
			Messages::Debug("Item Removed on Client");
	}
}

void Client::HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	std::string itemName = "Unknown";
	std::string itemDescription = "<UNKNOWN DESCRIPTION>";
	std::string itemIconFilePath = "none";
	int itemWeight = 0;
	int itemType = -1;
	float weaponDamage = 0.0f;
	float weaponRange = 0.0f;
	float projectileDamage = 0.0f;
	float projectileVelocity = 0.0f;
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
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
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
		MaloW::Debug("Wrong or no Item Type sent from server in Client::HandleAddInventoryItem ItemType: " + MaloW::convertNrToString(itemType));
		return;
	}
	//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
	Item* item = NULL;
	switch (itemType)
	{
	case ITEM_TYPE_FOOD_DEER_MEAT:
		item = new Food(ID, itemType, hunger);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED_BOW:
		item = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED_ROCK:
		item = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE_AXE:
		item = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
		item = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_CONTAINER_CANTEEN:
		item = new Container(ID, itemType, maxUse, currUse);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_PROJECTILE_ARROW:
		item = new Projectile(ID, itemType, projectileVelocity, projectileDamage);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_HEAD:
		item = new Gear(ID, itemType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_CHEST:
		item = new Gear(ID, itemType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_LEGS:
		item = new Gear(ID, itemType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_BOOTS:
		item = new Gear(ID, itemType);
		item->SetItemName(itemName);
		item->SetItemWeight(itemWeight);
		item->SetIconPath(itemIconFilePath);
		item->SetItemDescription(itemDescription);
		break;
	default:
		break;
	}
	if (this->zPlayerInventory->AddItem(item))
	{
		Gui_Item_Data gid = Gui_Item_Data(ID, itemName, itemIconFilePath, itemDescription);
		this->zGuiManager->AddInventoryItemToGui(gid);
		if (Messages::FileWrite())
		{
			Messages::Debug("Added Image ID: " + MaloW::convertNrToString(ID));
		}
	}
	else
	{
		if (item)
		{
			delete item;
			item = NULL;
		}
	}
}