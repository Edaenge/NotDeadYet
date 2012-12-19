#include "WeaponObject.h"



WeaponObject::WeaponObject() : StaticObjectActor()
{
	this->zWeight = 1;
	this->zRange = 1.0f;
	this->zDamage = 0.0f;
}
WeaponObject::WeaponObject( const WeaponData* data ) : StaticObjectActor()
{
	if(!data)
	{
		this->zWeight = 1;
		this->zRange = 1.0f;
		this->zDamage = 0.0f;
	}
	else
	{
		this->zWeight = data->zWeight;
		this->zDamage = data->zDamage;
		this->zRange = data->zRange;
		this->zActorModel = data->zModel;
		this->zActorObjectName = data->zObjName;
	}
}

WeaponObject::~WeaponObject()
{

}
