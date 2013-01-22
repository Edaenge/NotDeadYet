/*
Made by Edänge Simon Datum(14/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"

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
	OBJECT_TYPE_PROJECTILE_ARROW,
	OBJECT_TYPE_MATERIAL_SMALL_STICK,
	OBJECT_TYPE_MATERIAL_MEDIUM_STICK,
	OBJECT_TYPE_MATERIAL_LARGE_STICK,
	OBJECT_TYPE_MATERIAL_THREAD
};
/*Class for Static objects, such as Food, Weapons basically things that cannot move.*/
class StaticObjectActor : public Actor
{
public:
	StaticObjectActor(bool genID = false);
	StaticObjectActor(const std::string& meshModel, bool genID = false);
	virtual ~StaticObjectActor();
	
	virtual void Update(float deltaTime){};

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}
	/*! Returns number of stacks the object has.*/
	int GetStackSize() const {return this->zStacks;}
	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}
	void SetStackSize(const int size) {this->zStacks = size;}
	inline void SetPosition(const Vector3& pos) {zPos = pos;}
	inline void SetRotation(const Vector4& rot) {zRot = rot;}
	virtual void SetScale(const Vector3& scale) {zScale = scale;}
	void ModifyStackSize(const int size) {this->zStacks += size;}

protected:
	int zWeight;
	int zType;
	int zStacks;
	std::string zDescription;
	std::string zIconPath;

	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;

private:
};