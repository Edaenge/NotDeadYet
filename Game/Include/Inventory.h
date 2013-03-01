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
#include "Bandage.h"
#include <vector>
#include "Observer.h"

enum EQUIPMENT_SLOT
{
	EQUIPMENT_SLOT_MELEE_WEAPON,
	EQUIPMENT_SLOT_RANGED_WEAPON,
	EQUIPMENT_SLOT_PROJECTILE,
	EQUIPMENT_SLOT_HEAD,
	EQUIPMENT_SLOT_CHEST,
	EQUIPMENT_SLOT_LEGS,
	EQUIPMENT_SLOT_BOOTS
};

/*! Class That keeps track of the inventory data*/
class Inventory
{
public:
	Inventory();
	Inventory(const unsigned int inventorySize);
	virtual ~Inventory();
	/*! Search for the Item in and returns its position*/
	int Search(const unsigned int ID) const;
	int Search(Item* item) const;
	/*! Adds an item to the array*/
	bool AddItem(Item* item, bool &stacked);
	/*! Calculate how many slots this item can use.*/
	int CalcMaxAvailableSlots(Item* item);
	/*! Check if the item will stack if added.*/
	bool IsStacking(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(const unsigned int index) const;
	/*! Returns the Items*/
	std::vector<Item*> GetItems() const;
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID) const;
	/*! Removes the item from the list. Returns the removed item.*/
	Item* RemoveItem(const unsigned int ID);
	Item* RemoveItem(Item* item);
	/*! Returns blocked slots*/
	std::vector<bool> GetBlockedSlots() const;
	/*! Get the status of the slot at Position*/
	bool GetBlockedSlot(unsigned int index) const;
	/*! Returns the Inventory Capacity == Max Weight allowed*/
	int GetInventoryCapacity() const;
	/*! Returns the total Weight.*/
	inline int GetTotalWeight() const {return zWeightTotal;}
	/*! Returns amount of Items the player has in the inventory with itemType = TYPE*/
	int SearchForItemType(const int Type, const int SubType);
	/*! Removes weight from Inventory depending on item stacks and Weight.*/
	bool RemoveItemStack(const unsigned int ID, const unsigned int numberOfStacks = 1);
	/*! Search after the item and returns it if found else return NULL.*/
	Item* SearchAndGetItemFromType(const int Type, const int SubType);

	//Equipment
	/*! Equips Weapon, returns previous equipped item.*/
	Item* EquipRangedWeapon(RangedWeapon* weapon, bool& success);
	Item* EquipMeleeWeapon(MeleeWeapon* weapon, bool& success);

	Item* EquipProjectile(Projectile* projectile);
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

	/*! Swaps between the Primary and secondary Weapons.*/
	bool SwapWeapon();
private:
	/*Removes the item from the list.*/
	Item* Erase(const unsigned int Index);
	/*! Array of items*/
	std::vector<Item*> zItems;
	/*! Nr of slots available in inventory.*/
	unsigned int zSlotsAvailable;
	/*! Max nr of slots in The Inventory array.*/
	unsigned int zInventoryCap;
	/*! Nr of slots Occupied == Current Weight.*/
	unsigned int zWeightTotal;

	//Equipment Variables
	Item* zPrimaryEquip;
	Item* zSecondaryEquip;
	Projectile* zProjectile;
	RangedWeapon* zRangedWeapon;
	MeleeWeapon* zMeleeWeapon;
	std::vector<Gear*> zGear;
};
