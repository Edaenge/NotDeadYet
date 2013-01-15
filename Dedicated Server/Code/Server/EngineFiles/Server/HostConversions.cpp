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

	this->SendRemoveItemMessage(pActor->GetID(), food_Item->GetID());

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

	this->SendRemoveItemMessage(pActor->GetID(), weapon_Item->GetID());

	this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

	this->SendNewObjectMessage(weaponObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Projectile* projectile_Item)
{
	StaticProjectileObject* projectileObj = NULL;/* = new WeaponObject(false);*/

	if (!this->CreateStaticObjectActor(projectile_Item->GetItemType(), &projectileObj))
	{
		MaloW::Debug("Failed to Create StaticObject Projectile");
		SAFE_DELETE(projectileObj);
		return false;
	}
	//Creates A New WeaponObject With an Id And Default Values
	projectileObj->SetID(projectile_Item->GetID());
	projectileObj->SetPosition(pActor->GetPosition());

	this->SendRemoveItemMessage(pActor->GetID(), projectile_Item->GetID());

	this->zActorHandler->AddNewStaticProjectileActor(projectileObj);

	this->SendNewObjectMessage(projectileObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Material* material_Item)
{
	//StaticProjectileObject* projectileObj = NULL;/* = new WeaponObject(false);*/

	//if (!this->CreateStaticObjectActor(material_Item->GetItemType(), &projectileObj))
	//{
	//	MaloW::Debug("Failed to Create StaticObject Projectile");
	//	SAFE_DELETE(projectileObj);
	//	return false;
	//}
	////Creates A New WeaponObject With an Id And Default Values
	//projectileObj->SetID(material_Item->GetID());
	//projectileObj->SetPosition(pActor->GetPosition());

	//this->SendRemoveItemMessage(pActor->GetID(), material_Item->GetID());

	//this->zActorHandler->AddNewStaticProjectileActor(projectileObj);

	//this->SendNewObjectMessage(projectileObj);

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

	SendRemoveItemMessage(pActor->GetID(), container_Item->GetID());

	this->zActorHandler->AddNewStaticContainerActor(containerObj);

	this->SendNewObjectMessage(containerObj);

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, FoodObject* foodObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), foodObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)foodObj->GetID());

	this->SendToAllClients(removeMsg);

	if(!this->zActorHandler->RemoveStaticFoodActor(foodObj->GetID()))
		MaloW::Debug("Failed to remove static object.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, WeaponObject* weaponObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), weaponObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)weaponObj->GetID());

	this->SendToAllClients(removeMsg);

	this->zActorHandler->RemoveStaticWeaponActor(weaponObj->GetID());

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, ContainerObject* containerObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), containerObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)containerObj->GetID());

	this->SendToAllClients(removeMsg);

	this->zActorHandler->RemoveStaticContainerActor(containerObj->GetID());

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, StaticProjectileObject* projectileObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), projectileObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)projectileObj->GetID());

	this->SendToAllClients(removeMsg);

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

	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_DYNAMIC_OBJECT, (float)dynamicProjObj->GetID());
	this->SendToAllClients(msg);

	Vector3 pos = staticProjObj->GetPosition();
	Vector3 scale = staticProjObj->GetScale();
	Vector4 rot = staticProjObj->GetRotation();

	msg =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)staticProjObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, staticProjObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)staticProjObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, staticProjObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)staticProjObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, staticProjObj->GetIconPath());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, staticProjObj->GetDescription());

	this->SendToAllClients(msg);
}