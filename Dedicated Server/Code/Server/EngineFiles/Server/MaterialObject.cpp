#include "MaterialObject.h"

MaterialObject::MaterialObject(const bool genID /*= false*/ ) : StaticObjectActor(genID)
{
	this->zCraftingType = -1;
	this->zRequiredStackToCraft = -1;
	this->zActorType = ACTOR_TYPE_STATIC_OBJECT_MATERIAL;
}

MaterialObject::MaterialObject(const MaterialObject& other, bool genID /*= false*/)
{
	if(genID)
		this->GenerateID();
	else
		this->SetID(other.GetID());

	this->zRot = other.zRot;
	this->zPos = other.zPos;
	this->zType = other.zType;
	this->zScale = other.zScale;
	this->zWeight = other.zWeight;
	this->zStacks = other.zStacks;
	this->zIconPath = other.zIconPath;
	this->zActorType = other.zActorType;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zCraftingType = other.zCraftingType;
	this->zActorObjectName = other.zActorObjectName;
	this->zRequiredStackToCraft = other.zRequiredStackToCraft;
}

MaterialObject::MaterialObject(const MaterialObject* other, bool genID /*= false*/)
{
	if(genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zType = other->zType;
	this->zScale = other->zScale;
	this->zWeight = other->zWeight;
	this->zStacks = other->zStacks;
	this->zIconPath = other->zIconPath;
	this->zActorType = other->zActorType;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zCraftingType = other->zCraftingType;
	this->zActorObjectName = other->zActorObjectName;
	this->zRequiredStackToCraft = other->zRequiredStackToCraft;
}

MaterialObject::~MaterialObject()
{

}
