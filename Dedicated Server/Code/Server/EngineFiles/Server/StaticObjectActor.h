/*
Made by Edänge Simon Datum(14/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include <vector>

static const enum M_OBJECT_TYPE
{
	OBJECT_TYPE_WEAPON_RANGED_BOW,
	OBJECT_TYPE_WEAPON_RANGED_ROCK,
	OBJECT_TYPE_WEAPON_MELEE_AXE,
	OBJECT_TYPE_WEAPON_MELEE_POCKET_KNIFE,
	OBJECT_TYPE_GEAR_HEAD,
	OBJECT_TYPE_GEAR_CHEST,
	OBJECT_TYPE_GEAR_LEGS,
	OBJECT_TYPE_GEAR_BOOTS,
	OBJECT_TYPE_CONTAINER_CANTEEN,
	OBJECT_TYPE_CONTAINER_WATER_BOTTLE,
	OBJECT_TYPE_FOOD_DEER_MEAT,
	OBJECT_TYPE_FOOD_WOLF_MEAT,
	OBJECT_TYPE_AMMO_ARROW
};
/*Class for Static objects, such as Food, Weapons basically things that cannot move.*/
class StaticObjectActor : public Actor
{
public:
	StaticObjectActor(bool genID = true);
	StaticObjectActor(const std::string& meshModel, bool genID = true);
	virtual ~StaticObjectActor();
	
	virtual void Update(float deltaTime){};

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}

protected:
	int zWeight;
	int zType;
	std::string zDescription;
	std::string zIconPath;
private:
};