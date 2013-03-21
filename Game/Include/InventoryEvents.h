#pragma once

#include "Item.h"

class InventoryAddItemEvent : public Event
{
public:
	virtual ~InventoryAddItemEvent() {}

	Item* item;
};

class InventoryRemoveItemEvent : public Event
{
public:
	virtual ~InventoryRemoveItemEvent() {}

	Item* item;
};

class InventoryEquipItemEvent : public Event
{
public:
	virtual ~InventoryEquipItemEvent() {}

	Item* item;
};

class InventoryUnEquipItemEvent : public Event
{
public:
	virtual ~InventoryUnEquipItemEvent() {}

	Item* item;
};

class InventoryBindPrimaryWeapon : public Event
{
public:
	virtual ~InventoryBindPrimaryWeapon() {}

	Item* item;
	unsigned int ID;
};

class InventoryUnBindPrimaryWeapon : public Event
{
public:
	virtual ~InventoryUnBindPrimaryWeapon() {}

	Item* item;
	unsigned int ID;
};