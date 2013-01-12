#include "Host.h"

bool Host::CreateObjectFromItem(PlayerActor* pActor, Food* food_Item)
{
	FoodObject* foodObj = NULL;/* = new FoodObject(false);*/
	if (!this->CreateStaticObjectActor(food_Item->GetItemType(), &foodObj))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(foodObj);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	foodObj->SetID(food_Item->GetID());
	foodObj->SetPosition(pActor->GetPosition());

	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, food_Item->GetID());

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->AddNewStaticFoodActor(foodObj);

	this->SendNewObjectMessage(foodObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Weapon* weapon_Item)
{
	WeaponObject* weaponObj = NULL;/* = new WeaponObject(false);*/

	if (!this->CreateStaticObjectActor(weapon_Item->GetItemType(), &weaponObj))
	{
		MaloW::Debug("Failed to Create StaticObject Weapon");
		SAFE_DELETE(weaponObj);
		return false;
	}
	//Creates A New WeaponObject With an Id And Default Values
	weaponObj->SetID(weapon_Item->GetID());
	weaponObj->SetPosition(pActor->GetPosition());

	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, weapon_Item->GetID());

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

	this->SendNewObjectMessage(weaponObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Container* container_Item)
{
	ContainerObject* containerObj = NULL; /*= new ContainerObject(false);*/

	if (!this->CreateStaticObjectActor(container_Item->GetItemType(), &containerObj))
	{
		MaloW::Debug("Failed to Create StaticObject Container");
		SAFE_DELETE(containerObj);
		return false;
	}
	//Creates A New ContainerObject With an Id And Default Values
	containerObj->SetID(container_Item->GetID());
	containerObj->SetPosition(pActor->GetPosition());

	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, container_Item->GetID());

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->AddNewStaticContainerActor(containerObj);

	this->SendNewObjectMessage(containerObj);

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, FoodObject* foodObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, foodObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, foodObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, foodObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, foodObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, foodObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, foodObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_HUNGER, foodObj->GetHunger());

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, foodObj->GetID());

	this->SendToAllClients(removeMsg);

	this->SendToClient(pActor->GetID(), msg);

	if(!this->zActorHandler->RemoveStaticFoodActor(foodObj->GetID()))
		MaloW::Debug("Failed to remove static object.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, WeaponObject* weaponObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, weaponObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, weaponObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, weaponObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, weaponObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, weaponObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, weaponObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, weaponObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_RANGE, weaponObj->GetRange());

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, weaponObj->GetID());

	this->SendToAllClients(removeMsg);

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->RemoveStaticWeaponActor(weaponObj->GetID());

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, ContainerObject* containerObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, containerObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, containerObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, containerObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, containerObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, containerObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, containerObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_MAX, containerObj->GetMaxUses());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_CONTAINER_CURRENT, containerObj->GetCurrentUses());

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, containerObj->GetID());

	this->SendToAllClients(removeMsg);

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->RemoveStaticContainerActor(containerObj->GetID());

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, StaticProjectileObject* projectileObj)
{
	std::string msg;

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM, projectileObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, projectileObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, projectileObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, projectileObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, projectileObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, projectileObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, projectileObj->GetDamage());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, projectileObj->GetVelocity());

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, projectileObj->GetID());

	this->SendToAllClients(removeMsg);

	this->SendToClient(pActor->GetID(), msg);

	this->zActorHandler->RemoveStaticContainerActor(projectileObj->GetID());

	return true;
}

void Host::HandleConversion(DynamicProjectileObject* dynamicProjObj)
{
	StaticProjectileObject* staticProjObj = new StaticProjectileObject(dynamicProjObj, false);

	if (!staticProjObj)
	{
		MaloW::Debug("Failed to convert Dynamic Projectile to static");
		return;
	}

	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_DYNAMIC_OBJECT, dynamicProjObj->GetID());
	this->SendToAllClients(msg);

	Vector3 pos = staticProjObj->GetPosition();
	Vector3 scale = staticProjObj->GetScale();
	Vector4 rot = staticProjObj->GetRotation();

	msg =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, staticProjObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, staticProjObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, staticProjObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, staticProjObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, staticProjObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, staticProjObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, staticProjObj->GetDescription());

	this->SendToAllClients(msg);
}