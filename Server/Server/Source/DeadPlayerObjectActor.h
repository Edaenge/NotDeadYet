/*
	Made by Jensen Christopher Datum(24/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/
#pragma once

#include "DeadActor.h"
#include "Container.h"
#include "Food.h"
#include "Material.h"
#include "Gear.h"
#include "MeleeWeapon.h"
#include "RangedWeapon.h"

#include <vector>

class DeadPlayerObjectActor : public DeadActor
{
public:
	DeadPlayerObjectActor(const bool genID = false);

	void SetItems(std::vector<Item*> items) { this->zItems = items;}

private:

};