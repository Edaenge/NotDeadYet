/*
	Made by Jensen Christopher Datum(24/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "WorldObject.h"
#include <Gear.h>
#include <Food.h>
#include <Material.h>
#include <Container.h>
#include <MeleeWeapon.h>
#include <RangedWeapon.h>

#include <vector>

class DeadPlayerObject : public WorldObject
{
public:
	DeadPlayerObject(const long ID) : WorldObject(ID) {}
	virtual ~DeadPlayerObject() {}

	Item* GetItemAt(const int Index) const;
	Item* GetItem(const long ID) const;
	inline std::vector<Item*> GetItems() const {return this->zItems;}

	void SetItems(std::vector<Item*> items) { this->zItems = items;}

private:
	std::vector<Item*> zItems;
};