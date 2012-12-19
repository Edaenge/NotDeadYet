/*
Made by Edänge Simon Datum(17/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include <vector>
#include "Items/Item.h"
#include "ServerSide/WeaponObject.h"
#include "ServerSide/FoodObject.h"

/*This class reads and stores WordObjects.
  The objects can be fetched by using enums from Item.h.
*/
class ObjectManager
{

public:
	ObjectManager();
	virtual ~ObjectManager();

	bool ReadObjects();
	/*! Returns a weapon object from given type.
		Returns null if it cannot be found.
	*/
	const WeaponObject* GetWeaponObject(const int type);
	/*! Returns a food object from given type.
		Returns null if it cannot be found.
	*/
	const FoodObject* GetFoodObject(const int type);

private:
	void TrimAndSet( char* ret );
	bool InterpCommand( char* command, char* key, WeaponObject* wp );
	bool InterpCommand( char* command, char* key, FoodObject* fd );
	const WeaponObject* SearchType(std::vector<WeaponObject*>& weapons, const int type) const;
	const FoodObject* SearchType(std::vector<FoodObject*>& food, const int type) const;

private:
	std::vector<WeaponObject*> zWeapons;
	std::vector<FoodObject*>	  zFood;

};