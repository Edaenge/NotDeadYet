#include "Host.h"
#include "../../../../../Source/GameFiles/ClientServerMessages.h"


void Host::HandleWeaponUse(PlayerActor* pActor, const int ItemID)
{
	Equipment* eq = pActor->GetEquipment();
	Weapon* weapon = eq->GetWeapon();

	if (!weapon)
	{
		SendErrorMessage(pActor->GetID(),"No Weapon is Equipped");
		return;
	}
	if (weapon->GetID() != ItemID)
	{
		MaloW::Debug("Server weapon isn't the same as Client Weapon");
	}


	float range;
	RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(weapon);

	if (rWpn)
	{
		int type = -1;
		//Create Dynamic Object with player direction
		Vector3 direction = pActor->GetDirection();
		if (rWpn->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
		{
			Projectile* projectile = eq->GetProjectile();

			if (projectile)
			{
				if (projectile->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
				{
					type = ITEM_TYPE_PROJECTILE_ARROW;
				}
				DynamicProjectileObject* projectileObj = new DynamicProjectileObject(true);

				if(!this->CreateDynamicObjectActor(type, &projectileObj, true))
				{
					MaloW::Debug("Failed to Create Projectile");
					SAFE_DELETE(projectileObj);
					return;
				}
				MaloW::Debug("Created Arrow With ID: " + MaloW::convertNrToString(projectileObj->GetID()));

				float damage = 0.0f;
				float velocity = 5.0f;
				rWpn->UseWeapon(range, damage);

				damage += projectile->GetDamage();
				velocity = projectile->GetVelocity();

				Vector3 position = pActor->GetPosition();
				projectileObj->SetPosition(position);
				projectileObj->SetDirection(direction);
				projectileObj->SetDamage(damage);
				projectileObj->SetVelocity(velocity);
				//Adds The Object To the Array
				this->zActorHandler->AddNewDynamicProjectileActor(projectileObj);

				SendNewObjectMessage(projectileObj);

				projectile->SetStackSize(projectile->GetStackSize() - 1);
				if (projectile->GetStackSize() <= 0)
				{
					SendRemoveEquipment(pActor->GetID(), projectile->GetID(), EQUIPMENT_SLOT_AMMO);

					if (projectile)
					{
						delete projectile;
						projectile = NULL;
					}
					eq->UnEquipProjectile();
				}
				return;
			}
			else
			{
				this->SendErrorMessage(pActor->GetID(), "No_Ammunition_Equipped");
				return;
			}
		}
		if (rWpn->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
		{
			type = ITEM_TYPE_WEAPON_RANGED_ROCK;

			DynamicProjectileObject* projectileObj = new DynamicProjectileObject(true);

			if(!this->CreateDynamicObjectActor(type, &projectileObj, true))
			{
				MaloW::Debug("Failed to Create Projectile");
				SAFE_DELETE(projectileObj);
				return;
			}
			MaloW::Debug("Created Rock With ID: " + MaloW::convertNrToString(projectileObj->GetID()));

			float damage = 0.0f;
			float velocity = 5.0f;
			rWpn->UseWeapon(range, damage);

			Vector3 position = pActor->GetPosition();
			projectileObj->SetPosition(position);
			projectileObj->SetDirection(direction);
			projectileObj->SetDamage(damage);
			projectileObj->SetVelocity(velocity);
			//Adds The Object To the Array
			this->zActorHandler->AddNewDynamicProjectileActor(projectileObj);

			SendNewObjectMessage(projectileObj);

			rWpn->SetStackSize(rWpn->GetStackSize() - 1);
			if (rWpn->GetStackSize() <= 0)
			{
				SendRemoveEquipment(pActor->GetID(), rWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

				if (rWpn)
				{
					delete rWpn;
					rWpn = NULL;
				}
				eq->UnEquipWeapon();
			}
			return;
		}
	}

	MaloW::Debug("dynamic cast Failed in Host::WeaponUse (Ranged Weapon)");

	MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(weapon);

	if (mWpn)
	{
		float damage;
		mWpn->UseWeapon(range, damage);
		//Check Collision
		Vector3 direction = pActor->GetDirection();
		Vector3 position = pActor->GetPosition() + (direction * range);

		return;
	}

	MaloW::Debug("dynamic cast Failed in Host::WeaponUse (Melee Weapon)");

	return;
}

void Host::HandleItemUse(PlayerActor* pActor, const int ItemID)
{
	Inventory* inv = pActor->GetInventory();

	Item* item = inv->SearchAndGetItem(ItemID);

	if (!item)
	{
		this->SendErrorMessage(pActor->GetID(), "Item Doesn't Exist");
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
			this->SendErrorMessage(pActor->GetID(), "Stack is Empty");
			return;
		}
		pActor->EatFood(food->GetHunger());

		std::string msg;

		msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, food->GetID());
		this->SendToClient(pActor->GetID(), msg);
		if (food->GetStackSize() <= 0)
		{
			pActor->GetInventory()->RemoveItem(food);


		}


		return;
	}
	if (item->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || item->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
	{
		Container* container = dynamic_cast<Container*>(item);

		if (!container)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Container)");
			return;
		}

		if (!container->Use())
		{
			this->SendErrorMessage(pActor->GetID(), "Stack is Empty");
			return;
		}
		float hydration = (container->GetRemainingUses() + 1) * 15.0f;
		pActor->Drink(hydration);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, container->GetID());
		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK)
	{

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK)
	{
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK)
	{
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		Equipment* eq = pActor->GetEquipment();

		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);

		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Bow)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + rWpn->GetItemName());
		eq->EquipWeapon(rWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, rWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		Equipment* eq = pActor->GetEquipment();

		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);

		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Rock)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + rWpn->GetItemName());
		eq->EquipWeapon(rWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, rWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE)
	{
		Equipment* eq = pActor->GetEquipment();

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);

		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Axe)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + mWpn->GetItemName());
		eq->EquipWeapon(mWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, mWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
	{
		Equipment* eq = pActor->GetEquipment();

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);

		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Pocket Knife)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + mWpn->GetItemName());
		eq->EquipWeapon(mWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, mWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
	{
		Equipment* eq = pActor->GetEquipment();

		Projectile* arrow = dynamic_cast<Projectile*>(item);

		if (!arrow)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Arrow)");
			return;
		}

		inv->EquipItem(ItemID);

		Projectile* oldProjectile = eq->GetProjectile();


		if (oldProjectile)
		{
			inv->AddItem(oldProjectile);
		}
		MaloW::Debug("Weapon Equipped " + arrow->GetItemName());
		eq->EquipProjectile(arrow);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, arrow->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_AMMO);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
}

void Host::SendRemoveEquipment(const int ClientID, const int ItemID, const int Slot)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_EQUIPMENT, ItemID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, Slot);

	this->SendToClient(ClientID, msg);
}

void Host::HandleDropItem(PlayerActor* pActor, const int ItemID)
{	
	Item* item = NULL;
	item = pActor->GetItem(ItemID);

	if (!item)
	{
		this->SendErrorMessage(pActor->GetID(), "Failed To find Item");
		return;
	}
	int item_type = item->GetItemType();

	if (item_type == ITEM_TYPE_FOOD_DEER_MEAT)
	{
		Food* item_Food = dynamic_cast<Food*>(item);
		if (!item_Food)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Food);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}
		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_MELEE_AXE)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_CONTAINER_CANTEEN)
	{
		Container* item_Container = dynamic_cast<Container*>(item);
		if (!item_Container)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Container);
		pActor->DropObject(ItemID);
		return;
	}
}

bool Host::HandlePickupItem(PlayerActor* pActor, const int ObjectID)
{
	if(!HasClients())
		return false;

	//Check For FoodObject
	FoodObject* food = dynamic_cast<FoodObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_FOOD));

	if (food)
	{
		if (!pActor->PickUpObject(food))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Food " + food->GetActorObjectName());
			return false;
		}
		this->CreateItemFromObject(pActor, food);

		return true;
	}

	//Check For Weapon Object
	WeaponObject* weapon = dynamic_cast<WeaponObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_WEAPON));

	if (weapon)
	{
		if (!pActor->PickUpObject(weapon))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Weapon " + weapon->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, weapon);

		return true;
	}

	//Check For Container Object
	ContainerObject* container = dynamic_cast<ContainerObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_CONTAINER));

	if (container)
	{
		if (!pActor->PickUpObject(container))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Container " + container->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, container);

		return true;
	}

	//Check For Container Object
	StaticProjectileObject* projectile = dynamic_cast<StaticProjectileObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_PROJECTILE));

	if (projectile)
	{
		if (!pActor->PickUpObject(projectile))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Projectile" + projectile->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, projectile);

		return true;
	}

	this->SendErrorMessage(pActor->GetID(), "Couldn't_Pickup_Object");

	return false;
}

void Host::HandleUnEquipItem(PlayerActor* pActor, const int ItemID, const int Slot)
{
	if (Slot == -1)
	{
		MaloW::Debug("Error In Host::UnEquip Item Slot is -1");
		return;
	}

	Inventory* inv = pActor->GetInventory();
	Equipment* eq = pActor->GetEquipment();

	if (Slot == EQUIPMENT_SLOT_AMMO)
	{
		Projectile* projectile = eq->GetProjectile();

		if (projectile)
		{
			if (projectile->GetID() == ItemID)
			{
				eq->UnEquipProjectile();

				inv->AddItem(projectile);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

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

				inv->AddItem(wpn);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

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

				inv->AddItem(head);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

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

				inv->AddItem(chest);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

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

				inv->AddItem(legs);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

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

				inv->AddItem(boots);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

}

void Host::SendUnEquipMessage(const int PlayerID, const int ID, const int Slot)
{
	std::string message = this->zMessageConverter.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, ID);
	message += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, Slot);

	this->SendToClient(PlayerID, message);
}

Item* Host::CreateItemWithDefaultValues(const int ItemType)
{
	if (ItemType == ITEM_TYPE_PROJECTILE_ARROW)
	{
		StaticProjectileObject* new_Arrow = NULL;
		this->CreateStaticObjectActor(ItemType, &new_Arrow, true);

		if (!new_Arrow)
		{
			MaloW::Debug("Unable To Create Arrow Actor From Crafting");
			return NULL;
		}

		Projectile* arrow = new Projectile(new_Arrow->GetID(), new_Arrow->GetType(),
			new_Arrow->GetVelocity(), new_Arrow->GetDamage());

		arrow->SetStacking(true);
		arrow->SetItemWeight(new_Arrow->GetWeight());
		arrow->SetIconPath(new_Arrow->GetIconPath());
		arrow->SetStackSize(new_Arrow->GetStackSize());
		arrow->SetItemName(new_Arrow->GetActorObjectName());
		arrow->SetItemDescription(new_Arrow->GetDescription());

		SAFE_DELETE(new_Arrow);

		return arrow;
	}
	if (ItemType == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		WeaponObject* new_Bow = NULL;
		this->CreateStaticObjectActor(ItemType, &new_Bow, true);

		if (!new_Bow)
		{
			MaloW::Debug("Unable To Create Arrow Actor From Crafting");
			return NULL;
		}

		RangedWeapon* bow = new RangedWeapon(new_Bow->GetID(), new_Bow->GetType(), new_Bow->GetDamage(), new_Bow->GetRange());

		bow->SetStacking(false);
		bow->SetItemWeight(new_Bow->GetWeight());
		bow->SetIconPath(new_Bow->GetIconPath());
		bow->SetStackSize(new_Bow->GetStackSize());
		bow->SetItemName(new_Bow->GetActorObjectName());
		bow->SetItemDescription(new_Bow->GetDescription());

		SAFE_DELETE(new_Bow);
	}


	return NULL;
}