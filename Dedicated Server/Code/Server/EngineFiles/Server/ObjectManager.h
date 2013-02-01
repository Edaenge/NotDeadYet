/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "FileReader.h"
#include <vector>
#include "Item.h"
#include "WeaponObject.h"
#include "FoodObject.h"
#include "ContainerObject.h"
#include "MaterialObject.h"
#include "StaticProjectileObject.h"

/*This class reads and stores WordObjects.
  The objects can be fetched by using enums from Item.h.
*/
class ObjectManager : public FileReader
{
public:
	ObjectManager();
	virtual ~ObjectManager();

	bool ReadFromFile();
	/*! Returns a Weapon object from given type.
		Returns null if it cannot be found.
	*/
	const WeaponObject* GetWeaponObject(const int type);
	/*! Returns a Food object from given type.
		Returns null if it cannot be found.
	*/
	const FoodObject* GetFoodObject(const int type);
	/*! Returns a Container object from given type.
		Returns null if it cannot be found.
	*/
	const ContainerObject* GetContainerObject(const int type);
	/*! Returns a Container object from given type.
		Returns null if it cannot be found.
	*/
	const MaterialObject* GetMaterialObject(const int type);
	/*! Returns a Projectile object from given type.
		Returns null if it cannot be found.
	*/
	const StaticProjectileObject* GetStaticProjectileObject(const int type);
private:
	bool InterpCommand(char* command, char* key, WeaponObject* wp);
	bool InterpCommand(char* command, char* key, FoodObject* fd);
	bool InterpCommand(char* command, char* key, ContainerObject* fd);
	bool InterpCommand(char* command, char* key, StaticProjectileObject* fd);
	bool InterpCommand(char* command, char* key, MaterialObject* ma);

	const FoodObject* SearchType(std::vector<FoodObject*>& food, const int type) const;
	const WeaponObject* SearchType(std::vector<WeaponObject*>& weapons, const int type) const;
	const MaterialObject* SearchType(std::vector<MaterialObject*>& materials, const int type) const;
	const ContainerObject* SearchType(std::vector<ContainerObject*>& containers, const int type) const;
	const StaticProjectileObject* SearchType(std::vector<StaticProjectileObject*>& projectiles, const int type) const;

private:
	std::vector<FoodObject*> zFood;
	std::vector<WeaponObject*> zWeapons;
	std::vector<MaterialObject*> zMaterials;
	std::vector<ContainerObject*> zContainers;
	std::vector<StaticProjectileObject*> zStaticProjectiles;
};