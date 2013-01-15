#include "Host.h"
#include "../../../../../Source/GameFiles/ClientServerMessages.h"


void Host::HandleWeaponUse(PlayerActor* pActor, const int ItemID)
{
	Equipment* eq = pActor->GetEquipment();
	Weapon* weapon = eq->GetWeapon();

	if (!weapon)
	{
		SendErrorMessage(pActor->GetID(),"No_Weapon_is_Equipped");
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
				MaloW::Debug("Created Arrow With ID: " + MaloW::convertNrToString((float)projectileObj->GetID()));

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
					this->SendRemoveItemMessage(pActor->GetID(), projectile->GetID(), EQUIPMENT_SLOT_AMMO);

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
			MaloW::Debug("Created Rock With ID: " + MaloW::convertNrToString((float)projectileObj->GetID()));

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
				this->SendRemoveItemMessage(pActor->GetID(), rWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

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
		this->SendErrorMessage(pActor->GetID(), "Item_Doesn't_Exist");
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
			this->SendErrorMessage(pActor->GetID(), "Stack_is_Empty");
			return;
		}
		pActor->EatFood(food->GetHunger());

		std::string msg;

		msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, (float)food->GetID());
		this->SendToClient(pActor->GetID(), msg);
		if (food->GetStackSize() <= 0)
		{
			pActor->GetInventory()->RemoveItem(food);

			this->SendRemoveItemMessage(pActor->GetID(), food->GetID());
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
			this->SendErrorMessage(pActor->GetID(), "Stack_is_Empty");
			return;
		}
		float hydration = (container->GetRemainingUses() + 1) * 15.0f;
		pActor->Drink(hydration);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, (float)container->GetID());
		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK)
	{
		Material* crafting_Material = dynamic_cast<Material*>(item);
		if (!crafting_Material)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Material)");
			return;
		}
		if (!crafting_Material->Use())
		{
			this->SendErrorMessage(pActor->GetID(), "Not_Enough_Materials_To_Craft");
		}

		Item* item = CreateItemFromDefault(crafting_Material->GetCraftingType());

		if (!item)
		{
			MaloW::Debug("Failed to Create Item With Default Values in Host::UseItem (Small Stick)");
			return;
		}
		if (item->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
		{
			Projectile* new_Arrow = dynamic_cast<Projectile*>(item);

			if (!new_Arrow)
			{
				MaloW::Debug("dynamic cast Failed in Host::UseItem (Projectile)");
				return;
			}
			this->SendAddInventoryItemMessage(pActor->GetID(), new_Arrow);

			return;
		}
		
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK)
	{
		MediumStick* material_Medium_Stick = dynamic_cast<MediumStick*>(item);
		if (!material_Medium_Stick)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Medium Stick)");
			return;
		}
		Inventory* inventory = pActor->GetInventory();
		Item* thread = inventory->SearchAndGetItemFromType(ITEM_TYPE_MATERIAL_THREAD);

		if (!thread)
		{
			this->SendErrorMessage(pActor->GetID(), "Missing_Material_Thread_in_Inventory");
			return;
		}
		Thread* material_Thread = dynamic_cast<Thread*>(item);

		if (!material_Thread)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Thread)");
			return;
		}
		if (!material_Medium_Stick->IsUsable() || !material_Thread->IsUsable())
		{
			this->SendErrorMessage(pActor->GetID(), "Not_Enough_Materials_To_Craft");
		}
		material_Medium_Stick->Use();
		thread->Use();

		Item* item = CreateItemFromDefault(material_Medium_Stick->GetCraftingType());

		if (!item)
		{
			MaloW::Debug("Failed to Create Item With Default Values in Host::UseItem (Medium Stick)");
			return;
		}

		if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
		{
			RangedWeapon* new_Bow = dynamic_cast<RangedWeapon*>(item);

			if (!new_Bow)
			{
				MaloW::Debug("dynamic cast Failed in Host::UseItem (Bow)");
				return;
			}

			this->SendAddInventoryItemMessage(pActor->GetID(), new_Bow);
		}
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

		this->SendEquipMessage(pActor->GetID(), rWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

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

		this->SendEquipMessage(pActor->GetID(), rWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

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

		this->SendEquipMessage(pActor->GetID(), mWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

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

		this->SendEquipMessage(pActor->GetID(), mWpn->GetID(), EQUIPMENT_SLOT_WEAPON);

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

		this->SendEquipMessage(pActor->GetID(), arrow->GetID(), EQUIPMENT_SLOT_AMMO);

		return;
	}
}

void Host::HandleDropItem(PlayerActor* pActor, const int ItemID)
{	
	Item* item = NULL;
	item = pActor->GetItem(ItemID);

	if (!item)
	{
		this->SendErrorMessage(pActor->GetID(), "Failed_To_find_Item");
		return;
	}
	int item_type = item->GetItemType();

	if (item_type == ITEM_TYPE_FOOD_DEER_MEAT || item_type == ITEM_TYPE_FOOD_WOLF_MEAT)
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
	if (item_type == ITEM_TYPE_WEAPON_RANGED_BOW || item_type == ITEM_TYPE_WEAPON_RANGED_ROCK || 
		item_type == ITEM_TYPE_WEAPON_MELEE_AXE || item_type == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
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
	if (item_type == ITEM_TYPE_PROJECTILE_ARROW)
	{
		Projectile* item_Weapon = dynamic_cast<Projectile*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	//if (item_type == ITEM_TYPE_MATERIAL_SMALL_STICK || item_type == ITEM_TYPE_MATERIAL_MEDIUM_STICK || 
	//	item_type == ITEM_TYPE_MATERIAL_LARGE_STICK || item_type == ITEM_TYPE_MATERIAL_THREAD)
	//{
	//	Material* item_Material = dynamic_cast<Material*>(item);
	//	if (!item_Material)
	//	{
	//		MaloW::Debug("Wrong Item Type Set");
	//		return;
	//	}
	//	this->CreateObjectFromItem(pActor, item_Material);
	//	pActor->DropObject(ItemID);
	//	return;
	//}
	if (item_type == ITEM_TYPE_CONTAINER_CANTEEN || item_type == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
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
			MaloW::Debug("Item With ID doesn't exist in Ammo ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
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
			MaloW::Debug("Item With ID doesn't exist in Weapon ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
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
			MaloW::Debug("Item With ID doesn't exist in Head Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
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
			MaloW::Debug("Item With ID doesn't exist in Chest Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
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
			MaloW::Debug("Item With ID doesn't exist in Legs Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
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
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
	}

}

void Host::SendUnEquipMessage(const int PlayerID, const int ID, const int Slot)
{
	std::string message = this->zMessageConverter.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)ID);
	message += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->SendToClient(PlayerID, message);
}

void Host::SendEquipMessage(const int PlayerID, const int ID, const int Slot)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, (float)ID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

	this->SendToClient(PlayerID, msg);
}

void Host::SendRemoveItemMessage(const int PlayerID, const int ID, const int Slot)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_EQUIPMENT, (float)ID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->SendToClient(PlayerID, msg);
}

void Host::SendRemoveItemMessage(const int PlayerID, const int ID)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, StaticProjectileObject* projectileObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)projectileObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)projectileObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, projectileObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, projectileObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, projectileObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)projectileObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectileObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectileObj->GetVelocity());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Projectile* projectile)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)projectile->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)projectile->GetItemType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, projectile->GetItemDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, projectile->GetItemName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)projectile->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, projectile->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectile->GetVelocity());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectile->GetDamage());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, ContainerObject* containerObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)containerObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)containerObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, containerObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, containerObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, containerObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)containerObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_MAX, (float)containerObj->GetMaxUses());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_CURRENT, (float)containerObj->GetCurrentUses());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, FoodObject* foodObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)foodObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)foodObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, foodObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, foodObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)foodObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, foodObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_HUNGER, foodObj->GetHunger());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, WeaponObject* weaponObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)weaponObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)weaponObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, weaponObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, weaponObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, weaponObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)weaponObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, weaponObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, weaponObj->GetRange());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Weapon* weapon)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)weapon->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)weapon->GetItemType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, weapon->GetItemDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, weapon->GetItemName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, weapon->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)weapon->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, weapon->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, weapon->GetRange());

	this->SendToClient(PlayerID, msg);
}

Item* Host::CreateItemFromDefault(const int ItemType)
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