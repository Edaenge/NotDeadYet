#include "Host.h"

bool Host::CreateObjectFromItem(PlayerActor* pActor, Food* food_Item)
{
	FoodObject* foodObj = NULL;
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

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticFoodActor(foodObj);

	this->SendNewObjectMessage(foodObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Weapon* weapon_Item)
{
	WeaponObject* weaponObj = NULL;

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

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticWeaponActor(weaponObj);

	this->SendNewObjectMessage(weaponObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Projectile* projectile_Item)
{
	StaticProjectileObject* projectileObj = NULL;

	if (!this->CreateStaticObjectActor(projectile_Item->GetItemType(), &projectileObj))
	{
		MaloW::Debug("Failed to Create StaticObject Projectile");
		SAFE_DELETE(projectileObj);
		return false;
	}
	//Creates A New StaticProjectileObject With an Id And Default Values
	projectileObj->SetID(projectile_Item->GetID());
	projectileObj->SetPosition(pActor->GetPosition());

	this->SendRemoveItemMessage(pActor->GetID(), projectile_Item->GetID());

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticProjectileActor(projectileObj);

	this->SendNewObjectMessage(projectileObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Material* material_Item)
{
	MaterialObject* materialObj = NULL;

	if (!this->CreateStaticObjectActor(material_Item->GetItemType(), &materialObj))
	{
		MaloW::Debug("Failed to Create StaticObject Projectile");
		SAFE_DELETE(materialObj);
		return false;
	}
	//Creates A New MaterialObject With an Id And Default Values
	materialObj->SetID(material_Item->GetID());
	materialObj->SetPosition(pActor->GetPosition());

	this->SendRemoveItemMessage(pActor->GetID(), material_Item->GetID());

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticMaterialObject(materialObj);

	this->SendNewObjectMessage(materialObj);

	return true;
}

bool Host::CreateObjectFromItem(PlayerActor* pActor, Container* container_Item)
{
	ContainerObject* containerObj = NULL;

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

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticContainerActor(containerObj);

	this->SendNewObjectMessage(containerObj);

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, FoodObject* foodObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), foodObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)foodObj->GetID());

	this->SendToAllClients(removeMsg);

	if(!this->zGameMode->GetActorHandlerPtr()->RemoveStaticFoodActor(foodObj->GetID()))
		MaloW::Debug("Failed to remove static object food.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, WeaponObject* weaponObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), weaponObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)weaponObj->GetID());

	this->SendToAllClients(removeMsg);

	if(this->zGameMode->GetActorHandlerPtr()->RemoveStaticWeaponActor(weaponObj->GetID()))
		MaloW::Debug("Failed to remove static object weapon.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, ContainerObject* containerObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), containerObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)containerObj->GetID());

	this->SendToAllClients(removeMsg);

	if(this->zGameMode->GetActorHandlerPtr()->RemoveStaticContainerActor(containerObj->GetID()))
		MaloW::Debug("Failed to remove static object container.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, StaticProjectileObject* projectileObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), projectileObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)projectileObj->GetID());

	this->SendToAllClients(removeMsg);

	if(this->zGameMode->GetActorHandlerPtr()->RemoveStaticProjectileActor(projectileObj->GetID()))
		MaloW::Debug("Failed to remove static object projectile.");

	return true;
}

bool Host::CreateItemFromObject(PlayerActor* pActor, MaterialObject* materialObj)
{
	this->SendAddInventoryItemMessage(pActor->GetID(), materialObj);

	std::string removeMsg = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_STATIC_OBJECT, (float)materialObj->GetID());

	this->SendToAllClients(removeMsg);

	if(this->zGameMode->GetActorHandlerPtr()->RemoveStaticMaterialActor(materialObj->GetID()))
		MaloW::Debug("Failed to remove static object material.");

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

	this->SendNewObjectMessage(staticProjObj);

	this->zGameMode->GetActorHandlerPtr()->AddNewStaticProjectileActor(staticProjObj);
}