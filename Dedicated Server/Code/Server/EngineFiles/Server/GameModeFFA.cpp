#include "GameModeFFA.h"
#include "ClientServerMessages.h"

GameModeFFA::GameModeFFA(World* worldPtr) : GameMode(worldPtr)
{

}

GameModeFFA::~GameModeFFA()
{

}

void GameModeFFA::Update(float dt)
{
	this->zActorHandler->UpdateObjects(dt);
}

void GameModeFFA::WinningCheck()
{
	throw std::exception("The method or operation is not implemented.");
}

bool GameModeFFA::InitiGameObjects()
{
	this->InitStaticObjects();
	this->InitAnimals();

	return true;
}

void GameModeFFA::InitAnimals()
{
	Vector3 position = this->CalculateSpawnPoint(0, 4, 10.0f);
	this->zActorHandler->CreateDeerAnimal(true, position);
}

void GameModeFFA::InitStaticObjects()
{
	int maxClients = 10;

	Vector3 position;
	Vector3 center;
	float radius = 3.5f;
	int numberOfObjects = 12;
	int counter = 0;
	int total = 0;
	for (int i = 0; i < maxClients; i++)
	{
		int currentPoint = i % maxClients;
		center = this->CalculateSpawnPoint(currentPoint, maxClients, 20);
		//Creating Food Objects
		FoodObject* foodObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_FOOD_DEER_MEAT, foodObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			foodObj->SetPosition(position);
			this->zActorHandler->AddNewStaticFoodActor(foodObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Deer Meat Object ID: " + MaloW::convertNrToString((float)foodObj->GetID()));

			counter++;
		}
		foodObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_FOOD_WOLF_MEAT, foodObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			foodObj->SetPosition(position);
			this->zActorHandler->AddNewStaticFoodActor(foodObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Wolf Meat Object ID: " + MaloW::convertNrToString((float)foodObj->GetID()));

			counter++;
		}
		//Creating Weapons
		WeaponObject* weaponObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_AXE, weaponObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			weaponObj->SetPosition(position);
			this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Axe Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

			counter++;
		}
		weaponObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_POCKET_KNIFE, weaponObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			weaponObj->SetPosition(position);
			this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Pocket Knife Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

			counter++;
		}
		weaponObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_BOW, weaponObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			weaponObj->SetPosition(position);
			this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Bow Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

			counter++;
		}
		weaponObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_ROCK, weaponObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			weaponObj->SetPosition(position);
			this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Rock Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

			counter++;
		}

		//Creating Projectiles
		StaticProjectileObject* projectileObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_PROJECTILE_ARROW, projectileObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			projectileObj->SetPosition(position);
			this->zActorHandler->AddNewStaticProjectileActor(projectileObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Arrow Object ID: " + MaloW::convertNrToString((float)projectileObj->GetID()));

			counter++;
		}

		//Creating Materials
		MaterialObject* materialObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_SMALL_STICK, materialObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			materialObj->SetPosition(position);
			this->zActorHandler->AddNewStaticMaterialObject(materialObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Small Stick Object ID: " + MaloW::convertNrToString((float)materialObj->GetID()));

			counter++;
		}
		materialObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_MEDIUM_STICK, materialObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			materialObj->SetPosition(position);
			this->zActorHandler->AddNewStaticMaterialObject(materialObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Medium Stick Object ID: " + MaloW::convertNrToString((float)materialObj->GetID()));

			counter++;
		}
		materialObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_THREAD, materialObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			materialObj->SetPosition(position);
			this->zActorHandler->AddNewStaticMaterialObject(materialObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Thread Object ID: " + MaloW::convertNrToString((float)materialObj->GetID()));

			counter++;
		}
		//Creating Containers
		ContainerObject* containerObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_CANTEEN, containerObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			containerObj->SetPosition(position);
			this->zActorHandler->AddNewStaticContainerActor(containerObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Canteen Object ID: " + MaloW::convertNrToString((float)containerObj->GetID()));

			counter++;
		}
		containerObj = NULL;
		if(zObjCreator->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_WATER_BOTTLE, containerObj, true))
		{
			position = this->CalculateSpawnPoint(counter, numberOfObjects, radius, center);

			containerObj->SetPosition(position);
			this->zActorHandler->AddNewStaticContainerActor(containerObj);

			if (Messages::FileWrite())
				Messages::Debug("Created Water Bottle Object ID: " + MaloW::convertNrToString((float)containerObj->GetID()));

			counter++;
		}
		total += counter;
		counter = 0;
	}
}