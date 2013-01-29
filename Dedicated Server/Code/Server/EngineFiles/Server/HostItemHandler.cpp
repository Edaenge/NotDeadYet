#include "Host.h"
#include "../../../../../Source/GameFiles/ClientServerMessages.h"

void Host::HandleWeaponUse(PlayerActor* pActor, const long ItemID)
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
				DynamicProjectileObject* projectileObj = NULL;

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
				projectileObj->SetDamage(damage);
				projectileObj->SetSpeed(velocity);
				projectileObj->SetUpVector(pActor->GetUpVector());

			    //Adds The Object To the Array
				this->zActorHandler->AddNewDynamicProjectileActor(projectileObj, pActor->GetDirection());

				SendNewObjectMessage(projectileObj);

				projectile->Use();
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


			DynamicProjectileObject* projectileObj = NULL;

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

			projectileObj->SetDamage(damage);
			projectileObj->SetSpeed(velocity);
			//Adds The Object To the Array
			this->zActorHandler->AddNewDynamicProjectileActor(projectileObj, pActor->GetDirection());

			SendNewObjectMessage(projectileObj);

			rWpn->SetStackSize(rWpn->GetStackSize() - 1);
			this->SendWeaponUseMessage(pActor->GetID(), rWpn->GetID());

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
		CollisionEvent cEvent = this->zActorHandler->CheckMeeleCollision(pActor, range);

		BioActor* pVictim = dynamic_cast<BioActor*>(this->zActorHandler->GetActor(cEvent.actor_victim_ID, cEvent.actor_victim_type));

		if (pVictim)
		{
			pVictim->TakeDamage(damage);
		}

		return;
	}

	MaloW::Debug("dynamic cast Failed in Host::WeaponUse (Melee Weapon)");

	return;
}

void Host::HandleItemUse(PlayerActor* pActor, const long ItemID)
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

		this->SendUseItem(pActor->GetID(), food->GetID());
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

		this->SendUseItem(pActor->GetID(), container->GetID());

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
		this->SendUseItem(pActor->GetID(), crafting_Material->GetID());

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
			pActor->GetInventory()->AddItem(new_Arrow);
			this->SendAddInventoryItemMessage(pActor->GetID(), new_Arrow);

			return;
		}
		
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK)
	{
		Material* material_Medium_Stick = dynamic_cast<Material*>(item);
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
		Material* material_Thread = dynamic_cast<Material*>(thread);

		if (!material_Thread)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Thread)");
			return;
		}
		if (!material_Medium_Stick->IsUsable() || !material_Thread->IsUsable())
		{
			this->SendErrorMessage(pActor->GetID(), "Not_Enough_Materials_To_Craft");
			return;
		}

		Item* item = CreateItemFromDefault(material_Medium_Stick->GetCraftingType());

		if (!item)
		{
			MaloW::Debug("Failed to Create Item With Default Values in Host::UseItem (Medium Stick)");
			return;
		}

		material_Medium_Stick->Use();
		material_Thread->Use();

		this->SendUseItem(pActor->GetID(), material_Medium_Stick->GetID());
		this->SendUseItem(pActor->GetID(), material_Thread->GetID());

		if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
		{
			RangedWeapon* new_Bow = dynamic_cast<RangedWeapon*>(item);

			if (!new_Bow)
			{
				MaloW::Debug("dynamic cast Failed in Host::UseItem (Bow)");
				return;
			}
			pActor->GetInventory()->AddItem(new_Bow);
			this->SendAddInventoryItemMessage(pActor->GetID(), new_Bow);

			return;
		}

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK)
	{
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_MATERIAL_THREAD)
	{
		Material* material_Thread = dynamic_cast<Material*>(item);
		if (!material_Thread)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Thread)");
			return;
		}
		Inventory* inventory = pActor->GetInventory();
		Item* mediumStick = inventory->SearchAndGetItemFromType(ITEM_TYPE_MATERIAL_MEDIUM_STICK);

		if (!mediumStick)
		{
			this->SendErrorMessage(pActor->GetID(), "Missing_Material_Thread_in_Inventory");
			return;
		}
		Material* material_Medium_Stick = dynamic_cast<Material*>(mediumStick);

		if (!material_Medium_Stick)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Medium Stick)");
			return;
		}
		if (!material_Thread->IsUsable() || !material_Medium_Stick->IsUsable())
		{
			this->SendErrorMessage(pActor->GetID(), "Not_Enough_Materials_To_Craft");
		}

		Item* item = CreateItemFromDefault(material_Thread->GetCraftingType());

		if (!item)
		{
			MaloW::Debug("Failed to Create Item With Default Values in Host::UseItem (Medium Stick)");
			return;
		}

		material_Medium_Stick->Use();
		material_Thread->Use();

		this->SendUseItem(pActor->GetID(), material_Medium_Stick->GetID());
		this->SendUseItem(pActor->GetID(), material_Thread->GetID());

		if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
		{
			RangedWeapon* new_Bow = dynamic_cast<RangedWeapon*>(item);

			if (!new_Bow)
			{
				MaloW::Debug("dynamic cast Failed in Host::UseItem (Bow)");
				return;
			}
			pActor->GetInventory()->AddItem(new_Bow);
			this->SendAddInventoryItemMessage(pActor->GetID(), new_Bow);
		}
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

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			if(inv->AddItem(oldWeapon))
			{
				Projectile* oldProjectile = eq->GetProjectile();
				if (oldProjectile)
				{
					if (oldProjectile->GetItemType() != ITEM_TYPE_PROJECTILE_ARROW)
					{
						if(!inv->AddItem(oldProjectile))
						{
							MaloW::Debug("Failed to Add Projectile to inventory when Equipping Bow");
						}
					}
				}
			}
			else
			{
				this->SendErrorMessage(pActor->GetID(), "Failed_to_add_UnEquipped_weapon");
				return;
			}
		}
		MaloW::Debug("Weapon Equipped " + rWpn->GetItemName());

		inv->EquipItem(ItemID);
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

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			if(inv->AddItem(oldWeapon))
			{
				Projectile* oldProjectile = eq->GetProjectile();
				if (oldProjectile)
				{
					if(!inv->AddItem(oldProjectile))
					{
						MaloW::Debug("Failed to Add Projectile to inventory when Equipping Rock");
					}
				}
			}
		}
		MaloW::Debug("Weapon Equipped " + rWpn->GetItemName());

		inv->EquipItem(ItemID);
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


		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			if(inv->AddItem(oldWeapon))
			{
				Projectile* oldProjectile = eq->GetProjectile();
				if (oldProjectile)
				{
					if (oldProjectile->GetItemType() != ITEM_TYPE_PROJECTILE_ARROW)
					{
						if(!inv->AddItem(oldProjectile))
						{
							MaloW::Debug("Failed to Add Projectile to inventory when Equipping Rock");
						}
					}
				}
			}
		}
		MaloW::Debug("Weapon Equipped " + mWpn->GetItemName());

		inv->EquipItem(ItemID);
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
			if(inv->AddItem(oldWeapon))
			{
				Projectile* oldProjectile = eq->GetProjectile();
				if (oldProjectile)
				{
					if (oldProjectile->GetItemType() != ITEM_TYPE_PROJECTILE_ARROW)
					{
						if(!inv->AddItem(oldProjectile))
						{
							MaloW::Debug("Failed to Add Projectile to inventory when Equipping Rock");
						}
					}
				}
			}
		}
		MaloW::Debug("Weapon Equipped " + mWpn->GetItemName());
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

		
		Projectile* oldProjectile = eq->GetProjectile();

		if (oldProjectile)
		{
			if (oldProjectile->GetItemType() != arrow->GetItemType())
			{
				if(inv->AddItem(oldProjectile))
				{
					MaloW::Debug("Failed to Add Projectile to inventory when Equipping Arrow");
					return;
				}
			}
			else
			{
				int stacks = oldProjectile->GetStackSize() + arrow->GetStackSize();
				oldProjectile->SetStackSize(stacks);
			}
			
		}
		Weapon* weapon = eq->GetWeapon();
		if (weapon)
		{
			if (weapon->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
			{
				MaloW::Debug("Weapon Equipped " + arrow->GetItemName());
				inv->EquipItem(ItemID);
				eq->EquipProjectile(arrow);

				this->SendEquipMessage(pActor->GetID(), arrow->GetID(), EQUIPMENT_SLOT_AMMO);
				return;
			}
		}
		this->SendErrorMessage(pActor->GetID(), "Failed_to_Equip_Arrow_because_No_Weapon_is_Equipped");
		return;
	}
}

void Host::HandleDropItem(PlayerActor* pActor, const long ItemID)
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
	if (item_type == ITEM_TYPE_MATERIAL_SMALL_STICK || item_type == ITEM_TYPE_MATERIAL_MEDIUM_STICK || 
		item_type == ITEM_TYPE_MATERIAL_LARGE_STICK || item_type == ITEM_TYPE_MATERIAL_THREAD)
	{
		Material* item_Material = dynamic_cast<Material*>(item);
		if (!item_Material)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}
		this->CreateObjectFromItem(pActor, item_Material);
		pActor->DropObject(ItemID);
		return;
	}
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

bool Host::HandlePickupItem(PlayerActor* pActor, const long ObjectID)
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

	//Check For Projectile Object
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

	//Check For Material Object
	MaterialObject* material = dynamic_cast<MaterialObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_MATERIAL));

	if (material)
	{
		if (!pActor->PickUpObject(material))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_material" + material->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, material);

		return true;
	}
	this->SendErrorMessage(pActor->GetID(), "Couldn't_Pickup_Object");

	return false;
}

bool Host::HandleLootItem(PlayerActor* pActor, const int deadPlayerID, const long id, const int type)
{
	if (!HasClients())
		return false;

	DeadPlayerObjectActor* dpoActor = dynamic_cast<DeadPlayerObjectActor*>(this->zActorHandler->GetActor(deadPlayerID, ACTOR_TYPE_DEAD_PLAYER));

	if (!dpoActor)
	{
		MaloW::Debug("Failed to cast DeadPlayerObjectActor in Host::HandleLootItem");
		return false;
	}

	std::vector<Item*> items = dpoActor->GetItems();

	for (auto it = items.begin(); it < items.end(); it++)
	{
		if ((*it)->GetID() == id)
		{
			if ((*it)->GetItemType() != type)
			{
				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || (*it)->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
			{
				Food* food = dynamic_cast<Food*>((*it));
				if (!food)
				{
					MaloW::Debug("Failed to cast Food in Host::HandleLootItem");
					return false;
				}
				if (pActor->GetInventory()->AddItem(food))
				{
					SendAddInventoryItemMessage(pActor->GetID(), food);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}

				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || (*it)->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
			{
				Container* container = dynamic_cast<Container*>((*it));
				if (!container)
				{
					MaloW::Debug("Failed to cast Container in Host::HandleLootItem");
					return false;
				}
				if (pActor->GetInventory()->AddItem(container))
				{
					SendAddInventoryItemMessage(pActor->GetID(), container);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}
				
				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK ||
				(*it)->GetItemType() == ITEM_TYPE_MATERIAL_THREAD || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK)
			{
				Material* material = dynamic_cast<Material*>((*it));
				if (!material)
				{
					MaloW::Debug("Failed to cast Material in Host::HandleLootItem");
					return false;
				}
				if (pActor->GetInventory()->AddItem(material))
				{
					SendAddInventoryItemMessage(pActor->GetID(), material);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}

				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE || (*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
			{
				MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>((*it));
				if (!mWpn)
				{
					MaloW::Debug("Failed to cast Melee Weapon in Host::HandleLootItem");
					return false;
				}
				if (pActor->GetInventory()->AddItem(mWpn))
				{
					SendAddInventoryItemMessage(pActor->GetID(), mWpn);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}

				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW || (*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
			{
				RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>((*it));
				if (!rWpn)
				{
					MaloW::Debug("Failed to cast Ranged Weapon in Host::HandleLootItem");
					return false;
				}
				if (pActor->GetInventory()->AddItem(rWpn))
				{
					SendAddInventoryItemMessage(pActor->GetID(), rWpn);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}

				return false;
			}
			if ((*it)->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
			{
				Projectile* projectile = dynamic_cast<Projectile*>((*it));
				if (!projectile)
				{
					MaloW::Debug("Failed to cast Ranged Weapon in Host::HandleLootItem");
					return false;
				}

				if (pActor->GetInventory()->AddItem(projectile))
				{
					SendAddInventoryItemMessage(pActor->GetID(), projectile);
					items.erase(it);
					dpoActor->SetItems(items);
					return true;
				}

				return false;
			}
			return false;
		}
	}
	return false;
}

void Host::HandleUnEquipItem(PlayerActor* pActor, const long ItemID, const int Slot)
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
				if (projectile->GetID() == ItemID)
				{
					if(inv->AddItem(projectile))
					{
						eq->UnEquipProjectile();
						this->SendUnEquipMessage(pActor->GetID(), ItemID, EQUIPMENT_SLOT_AMMO);
					}
					return;
				}
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
				if(inv->AddItem(wpn))
				{
					eq->UnEquipWeapon();
					this->SendUnEquipMessage(pActor->GetID(), ItemID, EQUIPMENT_SLOT_WEAPON);
				}
				Projectile* projectile = eq->GetProjectile();

				if (projectile)
				{
					if(inv->AddItem(projectile))
					{
						eq->UnEquipProjectile();
						this->SendUnEquipMessage(pActor->GetID(), projectile->GetID(), EQUIPMENT_SLOT_AMMO);
					}
					return;
				}
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
				if(inv->AddItem(head))
				{
					eq->UnEquipGear(EQUIPMENT_SLOT_HEAD);
					this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);
				}
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
				if(inv->AddItem(chest))
				{
					eq->UnEquipGear(EQUIPMENT_SLOT_CHEST);
					this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);
				}
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
				if(inv->AddItem(legs))
				{
					eq->UnEquipGear(EQUIPMENT_SLOT_LEGS);
					this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);
				}
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
				if(inv->AddItem(boots))
				{
					eq->UnEquipGear(EQUIPMENT_SLOT_BOOTS);
					this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);
				}
				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString((float)ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString((float)Slot));
	}

}

void Host::SendWeaponUseMessage(const int PlayerID, const long ID)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_USE, (float)ID);

	this->SendToClient(PlayerID, msg);
}

void Host::SendUnEquipMessage(const int PlayerID, const long ID, const int Slot)
{
	std::string message = this->zMessageConverter.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)ID);
	message += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->SendToClient(PlayerID, message);
}

void Host::SendUseItem(const int PlayerID, const long ID)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
	
	this->SendToClient(PlayerID, msg);
}

void Host::SendEquipMessage(const int PlayerID, const long ID, const int Slot)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, (float)ID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->SendToClient(PlayerID, msg);
}

void Host::SendRemoveItemMessage(const int PlayerID, const long ID, const int Slot)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_EQUIPMENT, (float)ID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)Slot);

	this->SendToClient(PlayerID, msg);
}

void Host::SendRemoveItemMessage(const int PlayerID, const long ID)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
	this->SendToClient(PlayerID, msg);
}

void Host::SendRemoveDeadPlayerItem(const int PlayerID, const long ObjID, const long ItemID, const int Type)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_REMOVE_ITEM, (float)ObjID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ID, (float)ItemID);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)Type);

	this->SendToClient(PlayerID, msg, true);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, StaticProjectileObject* projectileObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)projectileObj->GetID());
	msg += this->AddItemMessage(projectileObj);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)projectileObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, projectileObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, projectileObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, projectileObj->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)projectileObj->GetStackSize());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)projectileObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectileObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectileObj->GetSpeed());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Projectile* projectile)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)projectile->GetID());

	Item* item = projectile;
	msg += this->AddItemMessage(item);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)projectile->GetItemType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, projectile->GetItemDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, projectile->GetItemName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)projectile->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)projectile->GetStackSize());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, projectile->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectile->GetVelocity());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectile->GetDamage());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, ContainerObject* containerObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)containerObj->GetID());
	msg += this->AddItemMessage(containerObj);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)containerObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, containerObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, containerObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, containerObj->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)containerObj->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)containerObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_MAX, (float)containerObj->GetMaxUses());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_CURRENT, (float)containerObj->GetCurrentUses());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Container* container)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)container->GetID());
	Item* item = container;
	msg += this->AddItemMessage(item);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)containerObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, containerObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, containerObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, containerObj->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)containerObj->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)containerObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_MAX, (float)container->GetMaxUses());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_CURRENT, (float)container->GetRemainingUses());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, FoodObject* foodObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)foodObj->GetID());
	msg += this->AddItemMessage(foodObj);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)foodObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, foodObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, foodObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)foodObj->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)foodObj->GetStackSize());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, foodObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_HUNGER, foodObj->GetHunger());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Food* food)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)food->GetID());
	Item* item = food;
	msg += this->AddItemMessage(item);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)foodObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, foodObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, foodObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)foodObj->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)foodObj->GetStackSize());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, foodObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_HUNGER, food->GetHunger());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, WeaponObject* weaponObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)weaponObj->GetID());
	msg += this->AddItemMessage(weaponObj);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)weaponObj->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, weaponObj->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, weaponObj->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, weaponObj->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)weaponObj->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)weaponObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, weaponObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, weaponObj->GetRange());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Weapon* weapon)
{
	std::string msg;
	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)weapon->GetID());
	Item* item = weapon;
	msg += this->AddItemMessage(item);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)weapon->GetItemType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, weapon->GetItemDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, weapon->GetItemName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, weapon->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)weapon->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)weapon->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, weapon->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, weapon->GetRange());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, MaterialObject* materialObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)materialObj->GetID());
	msg += this->AddItemMessage(materialObj);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)material->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, material->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, material->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, material->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)material->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)material->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE, (float)materialObj->GetCraftingType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED, (float)materialObj->GetRequiredStackToCraft());

	this->SendToClient(PlayerID, msg);
}

void Host::SendAddInventoryItemMessage(const int PlayerID, Material* material)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, (float)material->GetID());
	Item* item = material;
	msg += this->AddItemMessage(item);
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)material->GetType());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, material->GetDescription());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, material->GetActorObjectName());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, material->GetIconPath());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)material->GetWeight());
	//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)material->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE, (float)material->GetCraftingType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED, (float)material->GetRequiredStackToCraft());

	this->SendToClient(PlayerID, msg);
}

std::string Host::AddItemMessage(Food* food)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)food->GetID());
	Item* item = food;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_HUNGER, food->GetHunger());

	return msg;
}

std::string Host::AddItemMessage(Material* material)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)material->GetID());

	Item* item = material;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE, (float)material->GetCraftingType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED, (float)material->GetRequiredStackToCraft());


	return msg;
}

std::string Host::AddItemMessage(Container* container)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)container->GetID());

	Item* item = container;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_MAX, (float)container->GetMaxUses());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_CURRENT, (float)container->GetRemainingUses());

	return msg;
}

std::string Host::AddItemMessage(Projectile* projectile)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)projectile->GetID());
	
	Item* item = projectile;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectile->GetVelocity());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectile->GetDamage());

	return msg;
}

std::string Host::AddItemMessage(MeleeWeapon* meleeWeapon)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)meleeWeapon->GetID());

	Item* item = meleeWeapon;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, meleeWeapon->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, meleeWeapon->GetRange());

	return msg;
}

std::string Host::AddItemMessage(RangedWeapon* rangedWeapon)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)rangedWeapon->GetID());
	
	Item* item = rangedWeapon;
	msg += this->AddItemMessage(item);

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, rangedWeapon->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, rangedWeapon->GetRange());

	return msg;
}

std::string Host::AddItemMessage(Item* item)
{
	std::string msg = "";
	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)item->GetItemType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, item->GetItemDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, item->GetItemName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)item->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)item->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, item->GetIconPath());

	return msg;
}

std::string Host::AddItemMessage(StaticObjectActor* object)
{
	std::string msg = "";
	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)object->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, object->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, object->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)object->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)object->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, object->GetIconPath());

	return msg;
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
			new_Arrow->GetSpeed(), new_Arrow->GetDamage());

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

		return bow;
	}


	return NULL;
}