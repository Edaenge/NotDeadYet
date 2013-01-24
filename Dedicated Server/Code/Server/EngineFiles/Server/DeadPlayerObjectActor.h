/*
	Made by Jensen Christopher Datum(24/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "PhysicsActor.h"
#include "../../../../../Source/GameFiles/Items/Container.h"
#include "../../../../../Source/GameFiles/Items/Food.h"
#include "../../../../../Source/GameFiles/Items/Material.h"
#include "../../../../../Source/GameFiles/Items/Gear.h"
#include "../../../../../Source/GameFiles/Items/MeleeWeapon.h"
#include "../../../../../Source/GameFiles/Items/RangedWeapon.h"

class DeadPlayerObjectActor : public PhysicsActor
{
public:
	DeadPlayerObjectActor(const bool genID = true);
	virtual ~DeadPlayerObjectActor();

	Item* GetItemAt(const int Index) const;
	Item* GetItem(const long ID) const;
	std::vector<Item*> GetItems() const {return this->zItems;}

	void SetItems(std::vector<Item*> items) { this->zItems = items;}
private:
	std::vector<Item*> zItems;
};