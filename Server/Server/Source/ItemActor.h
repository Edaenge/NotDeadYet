/************************************/
/* Class that keep track of Objects */ 
/*      that can be Picked Up       */
/************************************/

#pragma once

#include "Actor.h"
#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "Projectile.h"
#include "Food.h"
#include "Container.h"
#include "Material.h"

class ItemActor : public Actor
{
public:
	ItemActor(Item* item, const std::string& model, const Vector3& position);
	virtual ~ItemActor();

	Item* GetItem() const {return this->zItem;}
private:
	Item* zItem;
};