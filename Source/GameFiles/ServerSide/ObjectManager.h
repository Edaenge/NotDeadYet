/*
Made by Edänge Simon Datum(17/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include <vector>

static const enum FOOD_OBJECT
{
	FOOD_OBJECT_MEAT
};

static const enum WEAPON_OBJECT
{
	WEAPON_OBJECT_SWORD
};

struct WeaponData
{
	WeaponData()
	{
		zRange	= 0.0f;
		zDamage = 0.0f;
		zWeight = 0;
		zType	= 0;
		
		zModel = "none";
	}

	float	zRange;
	float	zDamage;
	int		zWeight;
	int		zType;
	
	std::string zModel;
};

struct FoodData
{
	FoodData()
	{
		zHunger	= 0.0f;
		zWeight = 0;
		zType = 0;

		zModel = "none";
	}

	float	zHunger;
	int		zWeight;
	int		zType;

	std::string zModel;
};

/**/
class ObjectManager
{

public:
	ObjectManager();
	virtual ~ObjectManager();

	bool ReadObjects();
	/*! Returns a weapon object from given type.
		Returns null if it cannot be found.
	*/
	const WeaponData* GetWeaponObject(const int TYPE);
	/*! Returns a food object from given type.
		Returns null if it cannot be found.
	*/
	const FoodData* GetFoodObject(const int TYPE);

private:
	void TrimAndSet( char* ret );
	bool InterpCommand( const char* command, char* key, WeaponData& wp );
	bool InterpCommand( const char* command, char* key, FoodData& fd );
	const WeaponData* SearchType(std::vector<WeaponData>& weapons, const int type) const;
	const FoodData* SearchType(std::vector<FoodData>& food, const int type) const;

private:
	std::vector<WeaponData>	zWeapons;
	std::vector<FoodData>	zFood;

};