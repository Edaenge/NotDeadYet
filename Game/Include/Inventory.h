/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "Projectile.h"
#include "Gear.h"
#include "Food.h"
#include "Container.h"
#include "Material.h"
#include <vector>
#include "Observer.h"
enum EQUIPMENT_SLOT
{
	EQUIPMENT_SLOT_AMMO,
	EQUIPMENT_SLOT_RANGED_WEAPON,
	EQUIPMENT_SLOT_MELEE_WEAPON,
	EQUIPMENT_SLOT_HEAD,
	EQUIPMENT_SLOT_CHEST,
	EQUIPMENT_SLOT_LEGS,
	EQUIPMENT_SLOT_BOOTS
};

/*! Class That keeps track of the inventory data*/
class Inventory : public Observed
{
public:
	Inventory();
	Inventory(const unsigned int inventorySize);
	virtual ~Inventory();
	/*! Search for the Item in and returns its position*/
	int Search(const unsigned int ID) const;
	/*! Adds an item to the array*/
	bool AddItem(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(const unsigned int index) const;
	/*! Returns the Items*/
	std::vector<Item*> GetItems() const;
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID) const;
	/*! Deletes the item*/
	bool RemoveItem(const unsigned int index);
	bool RemoveItem(Item* item);
	/*! Returns blocked slots*/
	std::vector<bool> GetBlockedSlots() const;
	/*! Get the status of the slot at Position*/
	bool GetBlockedSlot(unsigned int index) const;
	/*! Returns the Inventory Capacity == Max Weight allowed*/
	int GetInventoryCapacity() const;
	/*! Returns amount of Items the player has in the inventory with itemType = TYPE*/
	int SearchForItemType(const int Type, const int SubType);
	/*! Removes a stack from Item*/
	bool RemoveItemStack(const unsigned int ID, const unsigned int numberOfStacks = 1);
	Item* SearchAndGetItemFromType(const int Type, const int SubType);
	/*! Returns Item and removes it from inventory
		Doesn't delete the Item.
	*/
	Item* EraseItem(const unsigned int ID);

	//Equipment
	/*! Equips Weapon*/
	void EquipRangedWeapon(RangedWeapon* weapon);
	void EquipMeleeWeapon(MeleeWeapon* weapon);

	void EquipProjectile(Projectile* projectile);
	void UnEquipProjectile();
	Projectile* GetProjectile();

	RangedWeapon* GetRangedWeapon();
	MeleeWeapon* GetMeleeWeapon();

	void UnEquipRangedWeapon();
	void UnEquipMeleeWeapon();

	/*! Returns the primary equipped item.*/
	Item* GetPrimaryEquip() const {return this->zPrimaryEquip;}
	Item* GetSecondaryEquip() const {return this->zSecondaryEquip;}
	/*! */
	void SetPrimaryEquip(unsigned int ID); 
	void SetSecondaryEquip(unsigned int ID);

	bool EquipGear(const unsigned int type, Gear* item);
	Gear* GetGear(const unsigned int type);
	void UnEquipGear(const unsigned int type);

private:
	/*! Array of items*/
	std::vector<Item*> zItems;
	/*! Array to keep track of Gui Representation*/
	std::vector<bool> zInventorySlotBlocked;
	/*! Nr of slots available in inventory == zInventorySlotBlocked array*/
	unsigned int zSlotsAvailable;
	/*! Max nr of slots in The zInventorySlotBlocked array*/
	unsigned int zInventoryCap;
	/*! Nr of slots Occupied == Current Weight*/
	unsigned int zWeightTotal;

	//Equipment Variables
	Item* zPrimaryEquip;
	Item* zSecondaryEquip;
	Projectile* zProjectile;
	RangedWeapon* zRangedWeapon;
	MeleeWeapon* zMeleeWeapon;
	std::vector<Gear*> zGear;
};
