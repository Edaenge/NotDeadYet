#include "Inventory.h"
#include "ClientServerMessages.h"
#include <MaloWFileDebug.h>
#include <Safe.h>

const unsigned int GEAR_SLOTS = 4;

Inventory::Inventory()
{
	this->zInventoryCap = 49;
	this->zWeightTotal = 0;
	
	this->zInventorySlotBlocked = std::vector<bool>();
	this->zItems = std::vector<Item*>();

	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotBlocked.push_back(false);
	}
	this->zSlotsAvailable = this->zInventoryCap;

	this->zRangedWeapon = NULL;
	this->zMeleeWeapon = NULL;
	this->zProjectile = NULL;
	this->zPrimaryEquip = NULL;
	this->zSecondaryEquip = NULL;

	for (unsigned int i = 0; i < GEAR_SLOTS; i++)
	{
		this->zGear.push_back(NULL);
	}
}

Inventory::Inventory(const unsigned int inventorySize)
{
	this->zInventoryCap = inventorySize;
	this->zWeightTotal = 0;

	this->zInventorySlotBlocked = std::vector<bool>();
	this->zItems = std::vector<Item*>();

	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotBlocked.push_back(false);
	}
	this->zSlotsAvailable = this->zInventoryCap;

	this->zRangedWeapon = NULL;
	this->zMeleeWeapon = NULL;
	this->zProjectile = NULL;
	this->zPrimaryEquip = NULL;
	this->zSecondaryEquip = NULL;
}

Inventory::~Inventory()
{
	//Remove Item Arrays
	for (auto x = this->zItems.begin(); x < this->zItems.end(); x++)
	{
		SAFE_DELETE((*x));

	}

	SAFE_DELETE(this->zRangedWeapon);
	SAFE_DELETE(this->zMeleeWeapon);
	SAFE_DELETE(this->zProjectile);

	for (auto x = this->zGear.begin(); x < this->zGear.end(); x++)
	{
		SAFE_DELETE((*x));
	}
}

std::vector<Item*> Inventory::GetItems() const
{
	return this->zItems;
}

bool Inventory::AddItem(Item* item)
{
	if (!item)
		return false;

	unsigned int weight = item->GetWeight() * item->GetStackSize();
	if(this->zWeightTotal + weight <= this->zInventoryCap)
	{
		this->zWeightTotal += weight;

		if (item->GetStacking())
		{
			Item* existingItem = this->SearchAndGetItemFromType(item->GetItemType(), item->GetItemSubType());
			if (existingItem)
			{
				if (existingItem->GetItemType() == item->GetItemType())
				{
					existingItem->IncreaseStackSize(item->GetStackSize());
					if (Messages::FileWrite())
						Messages::Debug("Added Stack to inventory " + item->GetItemName());

					return true;
				}
			}
		}
		//this->NotifyObservers()
		this->zItems.push_back(item);

		if (Messages::FileWrite())
			Messages::Debug("Added Item " + item->GetItemName() + " ID: " + MaloW::convertNrToString((float)item->GetID()));

		return true;
	}

	MaloW::Debug("Failed to add item "+item->GetItemName() + " ID: " +MaloW::convertNrToString((float)item->GetID()));
	return false;
}

Item* Inventory::SearchAndGetItem(unsigned int ID) const
{
	int position = this->Search(ID);

	return this->GetItem(position);
}

Item* Inventory::GetItem(const unsigned int index) const
{
	if (index < this->zItems.size())
	{
		return this->zItems[index];
	}
	return 0;
}

int Inventory::Search(const unsigned int ID) const
{
	for (unsigned int i = 0; i < this->zItems.size(); i++)
	{
		Item* temp = this->zItems[i];
		if (temp)
		{
			if (ID == temp->GetID())
				return i;
		}
	}
	return -1;
}

bool Inventory::RemoveItemStack(const unsigned int ID, const unsigned int numberOfStacks)
{
	int index = this->Search(ID);
	if (index == -1)
		return false;

	if ((unsigned int)index < this->zItems.size())
	{
		int weight = GetItem(index)->GetWeight() * numberOfStacks;
		this->zWeightTotal -= weight;

		if (Messages::FileWrite())
		{
			Messages::Debug("Removed Stacks: " + MaloW::convertNrToString((float)numberOfStacks) + " TotalWeight: " + MaloW::convertNrToString((float)zWeightTotal));
		}
		return true;
	}
	return false;
}

bool Inventory::RemoveItem(const unsigned int index)
{
	if (index < this->zItems.size())
	{
		int weight = GetItem(index)->GetWeight();
		this->zWeightTotal -= weight;

		Item* item = this->zItems.at(index);
		this->zItems.erase(this->zItems.begin() + index);

		SAFE_DELETE(item);

		return true;
	}
	return false;
}

bool Inventory::RemoveItem(Item* item)
{
	int index = this->Search(item->GetID());
	if (index != -1)
	{
		return this->RemoveItem(index);
	}
	return false;
}

std::vector<bool> Inventory::GetBlockedSlots() const
{
	return this->zInventorySlotBlocked;
}

bool Inventory::GetBlockedSlot(unsigned int index) const
{
	return this->zInventorySlotBlocked[index];
}

int Inventory::GetInventoryCapacity() const
{
	return this->zInventoryCap;
}

int Inventory::SearchForItemType(const int Type, const int SubType)
{
	int counter = 0;

	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == Type && (*it)->GetItemSubType() == SubType)
		{
			counter += (*it)->GetStackSize();
		}
	}

	return counter;
}

Item* Inventory::SearchAndGetItemFromType(const int Type, const int SubType)
{
	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == Type && (*it)->GetItemSubType() == SubType)
		{
			return (*it);
		}
	}
	return NULL;
}

Item* Inventory::EraseItem(const unsigned int ID)
{
	int index = this->Search(ID);

	if (index == -1)
		return NULL;
	
	if ((unsigned int)index < this->zItems.size())
	{
		Item* item = GetItem(index);
		int weight = item->GetWeight() * item->GetStackSize();
		this->zWeightTotal -= weight;

		/*for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}*/
		this->zItems.erase(this->zItems.begin() + index);

		return item;
	}
	return NULL;
}

//Equipment
void Inventory::EquipRangedWeapon(RangedWeapon* weapon)
{
	this->zRangedWeapon = weapon;

	if(!this->zPrimaryEquip)
		zPrimaryEquip = weapon;
	else
		zSecondaryEquip = weapon;
}

void Inventory::EquipMeleeWeapon(MeleeWeapon* weapon)
{
	this->zMeleeWeapon = weapon;

	if(!this->zPrimaryEquip)
		zPrimaryEquip = weapon;
	else
		zSecondaryEquip = weapon;
}

void Inventory::EquipProjectile(Projectile* projectile)
{
	if (!projectile)
		return;

	if (this->zProjectile)
	{
		if (Messages::FileWrite())
			Messages::Debug("Equipped projectile");

		if (this->zProjectile->GetItemType() == projectile->GetItemType())
		{
			int totalStacks = this->zProjectile->GetStackSize() + projectile->GetStackSize();
			this->zProjectile->SetStackSize(totalStacks);
		}
		else
		{
			this->zProjectile = projectile;
		}
	}
	else
	{
		this->zProjectile = projectile;
	}

	if(projectile->GetItemSubType() == ITEM_SUB_TYPE_ARROW)
		return;

	if(!zPrimaryEquip)
		this->zPrimaryEquip = projectile;
	if(!zSecondaryEquip)
		this->zSecondaryEquip = projectile;

}

bool Inventory::EquipGear(const unsigned int type, Gear* item)
{
	if (type < GEAR_SLOTS)
	{
		this->zGear[type] = item;
		return true;
	}
	return false;
}

Gear* Inventory::GetGear(const unsigned int type)
{
	if (type < GEAR_SLOTS)
	{
		return this->zGear[type];
	}
	return NULL;
}

void Inventory::UnEquipGear(const unsigned int type)
{
	if (type < GEAR_SLOTS)
	{
		this->zGear[type] = NULL;
	}
}

MeleeWeapon* Inventory::GetMeleeWeapon()
{
	return this->zMeleeWeapon;
}

RangedWeapon* Inventory::GetRangedWeapon()
{
	return this->zRangedWeapon;
}

void Inventory::UnEquipRangedWeapon()
{
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Weapon");

	if(zPrimaryEquip == zRangedWeapon)
	{
		zPrimaryEquip = NULL;
		zPrimaryEquip = zSecondaryEquip;
		zSecondaryEquip = NULL;
	}
	else
	{
		zSecondaryEquip = NULL;
	}

	this->zRangedWeapon = NULL;
}

void Inventory::UnEquipMeleeWeapon()
{
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Weapon");

	if(zPrimaryEquip == zMeleeWeapon)
	{
		zPrimaryEquip = NULL;
		zPrimaryEquip = zSecondaryEquip;
		zSecondaryEquip = NULL;
	}
	else
	{
		zSecondaryEquip = NULL;
	}

	this->zMeleeWeapon = NULL;
}

void Inventory::UnEquipProjectile()
{
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Projectile");

	if(zPrimaryEquip == zProjectile)
	{
		zPrimaryEquip = NULL;
		zPrimaryEquip = zSecondaryEquip;
		zSecondaryEquip = NULL;
	}
	else
	{
		zSecondaryEquip = NULL;
	}

	this->zProjectile = NULL;
}

Projectile* Inventory::GetProjectile()
{
	return this->zProjectile;
}

void Inventory::SetPrimaryEquip( unsigned int ID )
{
	Item* item = SearchAndGetItem(ID);

	if(!item)
	{
		MaloW::Debug("Cannot find item in SetPrimaryEquip.");
		return;
	}

	this->zPrimaryEquip = item;
}

void Inventory::SetSecondaryEquip( unsigned int ID )
{
	Item* item = SearchAndGetItem(ID);

	if(!item)
	{
		MaloW::Debug("Cannot find item in SetPrimaryEquip.");
		return;
	}

	this->zSecondaryEquip = item;
}
