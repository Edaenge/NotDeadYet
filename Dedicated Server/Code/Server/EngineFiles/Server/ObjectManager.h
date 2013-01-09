/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include <vector>
#include "../../../../../Source/GameFiles/Items/Item.h"
#include "WeaponObject.h"
#include "FoodObject.h"
#include "ContainerObject.h"

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
	/*! Returns a container object from given type.
		Returns null if it cannot be found.
	*/
	const ContainerObject* GetContainerObject(const int type);
private:
	void TrimAndSet(char* ret);
	bool InterpCommand(char* command, char* key, WeaponObject* wp);
	bool InterpCommand(char* command, char* key, FoodObject* fd);
	bool InterpCommand(char* command, char* key, ContainerObject* fd);
	const WeaponObject* SearchType(std::vector<WeaponObject*>& weapons, const int type) const;
	const FoodObject* SearchType(std::vector<FoodObject*>& food, const int type) const;
	const ContainerObject* SearchType(std::vector<ContainerObject*>& container, const int type) const;
	bool Replace(char* key);
private:
	std::vector<FoodObject*> zFood;
	std::vector<WeaponObject*> zWeapons;
	std::vector<ContainerObject*> zContainer;

};