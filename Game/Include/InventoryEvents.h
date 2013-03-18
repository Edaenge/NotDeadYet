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

	unsigned int ID;
};

class InventoryEquipItemEvent : public Event
{
public:
	virtual ~InventoryEquipItemEvent() {}

	unsigned int id;
	unsigned int slot;
};

class InventoryUnEquipItemEvent : public Event
{
public:
	virtual ~InventoryUnEquipItemEvent() {}

	unsigned int id;
	unsigned int slot;
};

class InventoryBindPrimaryWeapon : public Event
{
public:
	virtual ~InventoryBindPrimaryWeapon() {}

	std::string model;
	unsigned int ID;
	unsigned int type;
	unsigned int subType;
};

class InventoryUnBindPrimaryWeapon : public Event
{
public:
	virtual ~InventoryUnBindPrimaryWeapon() {}

	std::string model;
	unsigned int ID;
};