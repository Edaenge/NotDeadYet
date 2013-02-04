#include "ObjectCreator.h"
#include "RangedWeapon.h"

ObjectCreator::ObjectCreator(PhysicsEngine* zPhysicEngine)
{
	this->zObjectManager = new ObjectManager();
	this->zPhysicEngine = zPhysicEngine;
}

ObjectCreator::~ObjectCreator()
{
	SAFE_DELETE(this->zObjectManager);
}

bool ObjectCreator::InitObjectCreator()
{
	return this->zObjectManager->ReadFromFile();
}

bool ObjectCreator::CreateStaticObjectActor( const int type, FoodObject** foodObj, const bool genID /*= false*/ )
{
	//Get Default Values For a Meat Object
	const FoodObject* food = this->zObjectManager->GetFoodObject(type);

	if (!food)
		return false;

	*foodObj = new FoodObject(food, genID);

	return true;
}

bool ObjectCreator::CreateStaticObjectActor( const int type, WeaponObject** weaponObj, const bool genID /*= false*/ )
{
	//Get Default Values For a Weapon Object
	const WeaponObject* weapon = this->zObjectManager->GetWeaponObject(type);

	if (!weapon)
		return false;

	*weaponObj = new WeaponObject(weapon, genID);

	return true;
}

bool ObjectCreator::CreateStaticObjectActor( const int type, MaterialObject** materialObj, const bool genID /*= false*/ )
{
	//Get Default Values For a container Object
	const MaterialObject* material = this->zObjectManager->GetMaterialObject(type);

	if (!material)
		return false;

	*materialObj = new MaterialObject(material, genID);

	return true;
}

bool ObjectCreator::CreateStaticObjectActor( const int type, ContainerObject** containerObj, const bool genID /*= false*/ )
{
	//Get Default Values For a container Object
	const ContainerObject* container = this->zObjectManager->GetContainerObject(type);

	if (!container)
		return false;

	*containerObj = new ContainerObject(container, genID);

	return true;
}

bool ObjectCreator::CreateStaticObjectActor( const int type, StaticProjectileObject** projectileObj, const bool genID /*= false*/ )
{
	//Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zObjectManager->GetStaticProjectileObject(type);

	if (!projectile)
		return false;

	*projectileObj = new StaticProjectileObject(projectile, genID);

	return true;
}

bool ObjectCreator::CreateDynamicObjectActor( const int type, DynamicProjectileObject** projectileObj, bool genID )
{
	//Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zObjectManager->GetStaticProjectileObject(type);

	if (!projectile)
		return false;

	string modelPath = projectile->GetActorModel();

	(*projectileObj) = new DynamicProjectileObject(genID);

	PhysicsObject* pObj = this->zPhysicEngine->CreatePhysicsObject(modelPath, Vector3(0,0,0));

	(*projectileObj)->SetPhysicObject(pObj);

	//Creates A New ProjectileObject With an Id And Default Values 
	(*projectileObj)->SetType(type);
	(*projectileObj)->SetWeight(projectile->GetWeight());
	(*projectileObj)->SetDamage(projectile->GetDamage());
	(*projectileObj)->SetIconPath(projectile->GetIconPath());
	(*projectileObj)->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	(*projectileObj)->SetActorModel(modelPath);
	(*projectileObj)->SetDescription(projectile->GetDescription());
	(*projectileObj)->SetSpeed(projectile->GetSpeed());
	(*projectileObj)->SetActorObjectName(projectile->GetActorObjectName());

	return true;
}

bool ObjectCreator::CreateObjectFromItem( const Vector3& pos, Weapon* weapon_Item, WeaponObject** objOut )
{
	if(*objOut)
		SAFE_DELETE(*objOut);

	if (!this->CreateStaticObjectActor(weapon_Item->GetItemType(), objOut))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(*objOut);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	(*objOut)->SetID(weapon_Item->GetID());
	(*objOut)->SetPosition(pos);

	return true;
}

bool ObjectCreator::CreateObjectFromItem( const Vector3& pos, Food* food_Item, FoodObject** objOut )
{
	if(*objOut)
		SAFE_DELETE(*objOut);

	if (!this->CreateStaticObjectActor(food_Item->GetItemType(), objOut))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(*objOut);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	(*objOut)->SetID(food_Item->GetID());
	(*objOut)->SetPosition(pos);

	return true;
}

bool ObjectCreator::CreateObjectFromItem( const Vector3& pos, Container* container_item, ContainerObject** objOut )
{
	if(*objOut)
		SAFE_DELETE(*objOut);

	if (!this->CreateStaticObjectActor(container_item->GetItemType(), objOut))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(*objOut);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	(*objOut)->SetID(container_item->GetID());
	(*objOut)->SetPosition(pos);

	return true;
}

bool ObjectCreator::CreateObjectFromItem( const Vector3& pos, Projectile* projectile_Item, StaticProjectileObject** objOut )
{
	if(*objOut)
		SAFE_DELETE(*objOut);

	if (!this->CreateStaticObjectActor(projectile_Item->GetItemType(), objOut))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(*objOut);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	(*objOut)->SetID(projectile_Item->GetID());
	(*objOut)->SetPosition(pos);

	return true;
}

bool ObjectCreator::CreateObjectFromItem( const Vector3& pos, Material* material_Item, MaterialObject** objOut )
{
	if(*objOut)
		SAFE_DELETE(*objOut);

	if (!this->CreateStaticObjectActor(material_Item->GetItemType(), objOut))
	{
		MaloW::Debug("Failed to Create StaticObject Food");
		SAFE_DELETE(*objOut);
		return false;
	}

	//Creates A New FoodObject With an Id And Default Values 

	(*objOut)->SetID(material_Item->GetID());
	(*objOut)->SetPosition(pos);

	return true;
}

Item* ObjectCreator::CreateItemFromDefault( const int ItemType )
{
	if (ItemType == ITEM_TYPE_PROJECTILE_ARROW)
	{
		StaticProjectileObject* new_Arrow = NULL;
		this->CreateStaticObjectActor(ItemType, &new_Arrow, true);

		if (!new_Arrow)
		{
			MaloW::Debug("Unable To Create Arrow Actor From Crafting");
			return NULL;
		}

		Projectile* arrow = new Projectile(new_Arrow->GetID(), new_Arrow->GetType(),
			new_Arrow->GetSpeed(), new_Arrow->GetDamage());

		arrow->SetStacking(true);
		arrow->SetItemWeight(new_Arrow->GetWeight());
		arrow->SetIconPath(new_Arrow->GetIconPath());
		arrow->SetStackSize(new_Arrow->GetStackSize());
		arrow->SetItemName(new_Arrow->GetActorObjectName());
		arrow->SetItemDescription(new_Arrow->GetDescription());

		SAFE_DELETE(new_Arrow);

		return arrow;
	}
	if (ItemType == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		WeaponObject* new_Bow = NULL;
		this->CreateStaticObjectActor(ItemType, &new_Bow, true);

		if (!new_Bow)
		{
			MaloW::Debug("Unable To Create Arrow Actor From Crafting");
			return NULL;
		}

		RangedWeapon* bow = new RangedWeapon(new_Bow->GetID(), new_Bow->GetType(), new_Bow->GetDamage(), new_Bow->GetRange());

		bow->SetStacking(false);
		bow->SetItemWeight(new_Bow->GetWeight());
		bow->SetIconPath(new_Bow->GetIconPath());
		bow->SetStackSize(new_Bow->GetStackSize());
		bow->SetItemName(new_Bow->GetActorObjectName());
		bow->SetItemDescription(new_Bow->GetDescription());

		SAFE_DELETE(new_Bow);

		return bow;
	}
	return NULL;
}

void ObjectCreator::HandleConversion( DynamicProjectileObject* dynamicProjObj, StaticProjectileObject** objOut )
{
	StaticProjectileObject* staticProjObj = new StaticProjectileObject(dynamicProjObj, false);

	if (!staticProjObj)
	{
		MaloW::Debug("Failed to convert Dynamic Projectile to static");
		return;
	}
}