
#include "ActorHandler.h"

ActorHandler::ActorHandler(World* worldPtr)
{
	this->zObjManager = new ObjectManager();
	this->zObjManager->ReadFromFile();
	this->zPhysicsEngine = new PhysicsEngine();
	this->zWorld = worldPtr;
}

ActorHandler::~ActorHandler()
{
	for(auto x = this->zPlayers.begin(); x < this->zPlayers.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zAnimals.begin(); x < this->zAnimals.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zFoods.begin(); x < this->zFoods.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zWeapons.begin(); x < this->zWeapons.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zContainers.begin(); x < this->zContainers.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zMaterials.begin(); x < this->zMaterials.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zStaticProjectiles.begin(); x < this->zStaticProjectiles.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zDynamicProjectiles.begin(); x < this->zDynamicProjectiles.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	SAFE_DELETE(this->zObjManager);
	SAFE_DELETE(this->zPhysicsEngine);
}

void ActorHandler::UpdateObjects(float deltaTime)
{
	Vector3 entityPos[32+48]; //Number of players + number of animals
	float entityVelocity[32+48];
	float entityHealth[32+48];

	int nrOfPlayers = 0;
	int nrOfEntities = 0;

	//Update Players (and get info about them)
	for (auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		(*it)->Update(deltaTime);
		entityPos[nrOfEntities] = (*it)->GetPosition();
		entityVelocity[nrOfEntities] = (*it)->GetVelocity();
		entityHealth[nrOfEntities] = (*it)->GetHealth();

		nrOfPlayers++;
		nrOfEntities++;
	}

	//Get information about all animals ahead of updating them.
	for (auto it = this->zAnimals.begin(); it < this->zAnimals.end(); it++)
	{
		entityPos[nrOfEntities] = (*it)->GetPosition();
		entityVelocity[nrOfEntities] = (*it)->GetVelocity();
		entityHealth[nrOfEntities] = (*it)->GetHealth();
	
		nrOfEntities++;
	}
	
	//Update Animals (And set info about all the animals)
	for (auto it = this->zAnimals.begin(); it < this->zAnimals.end(); it++)
	{
		for(int i = 0; i < nrOfPlayers; i++) //Giving info about players	//Potential area for optimization, probably.
		{
			(*it)->SetTargetInfo(i,entityPos[i], entityVelocity[i], entityHealth[i]);
		}
		for(int i = nrOfEntities - nrOfPlayers; i < nrOfEntities; i++) //Giving info about other animals
		{	//WARNING: There might be some problems if an animal essentially gets itself, requires testing and coding to avoid schizo hypocondria.
			(*it)->SetTargetInfo(i,entityPos[i], entityVelocity[i], entityHealth[i], (*it)->GetType() ); 
		}
		
		(*it)->SetCurrentTargets(nrOfEntities);
		(*it)->Update(deltaTime);
	}
	//Update DynamicObjects
	for (auto it = this->zDynamicProjectiles.begin(); it < this->zDynamicProjectiles.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
}

bool ActorHandler::AddNewPlayer(PlayerActor* new_player)
{
	if(!new_player)
		return false;

	new_player->SetObjManager(this->zObjManager);
	this->zPlayers.push_back(new_player);

	return true;
}

bool ActorHandler::AddNewDeadPlayer( DeadPlayerObjectActor* new_DeadPlayer )
{
	if (!new_DeadPlayer)
		return false;

	this->zDeadPlayers.push_back(new_DeadPlayer);

	return true;
}

bool ActorHandler::AddNewAnimalActor(AnimalActor* new_animal)
{
	if(!new_animal)
		return false;

	this->zAnimals.push_back(new_animal);

	return true;
}

bool ActorHandler::AddNewDeadAnimal( DeadAnimalObjectActor* new_DeadAnimal )
{
	if (!new_DeadAnimal)
		return false;

	this->zDeadAnimals.push_back(new_DeadAnimal);

	return true;
}

bool ActorHandler::AddNewStaticFoodActor(FoodObject* new_Food)
{
	if(!new_Food)
		return false;

	this->zFoods.push_back(new_Food);

	return true;
}

bool ActorHandler::AddNewStaticWeaponActor(WeaponObject* new_Weapon)
{
	if(!new_Weapon)
		return false;

	this->zWeapons.push_back(new_Weapon);

	return true;
}

bool ActorHandler::AddNewStaticContainerActor(ContainerObject* new_Container)
{
	if(!new_Container)
		return false;

	this->zContainers.push_back(new_Container);

	return true;
}

bool ActorHandler::AddNewStaticProjectileActor(StaticProjectileObject* new_Projectile)
{
	if(!new_Projectile)
		return false;

	this->zStaticProjectiles.push_back(new_Projectile);

	return true;
}

bool ActorHandler::AddNewStaticMaterialObject(MaterialObject* new_Material)
{
	if(!new_Material)
		return false;

	this->zMaterials.push_back(new_Material);

	return true;
}

bool ActorHandler::AddNewDynamicProjectileActor( DynamicProjectileObject* new_Projectile, Vector3 direction )
{
	if(!new_Projectile)
		return false;

	PhysicsObject* pObj = new_Projectile->GetPhysicObject();

	if(!pObj)
		MaloW::Debug("Error in function AddNewDynamicProjectileActor in ActorHandler: PhysicObj is null.");

	//Move arrow
	Vector3 pos = new_Projectile->GetPosition();
	pos += direction * 2;
	pos.y += 1.75f;

	//Rotate mesh
	Vector3 ArrowDirection = new_Projectile->GetInitialDirection();
	Vector3 CameraDirection = pObj->GetVelocity();
	Vector3 around;
	float angle;

	ArrowDirection.Normalize();
	CameraDirection.Normalize();

	around = ArrowDirection.GetCrossProduct(CameraDirection);
	angle = acos(ArrowDirection.GetDotProduct(CameraDirection));// / (ArrowDirection.GetLength() * CameraDirection.GetLength()));

	pObj->SetQuaternion(Vector4(0,0,0,1));
	pObj->RotateAxis(around, angle);

	//Set Physic values
	Vector3 velocity = direction * new_Projectile->GetSpeed();
	pObj->SetMass(1.0f);
	pObj->SetVelocity(velocity);
	pObj->SetAcceleration(Vector3(.0f, -9.82f, 0.0f));
	pObj->SetDamping(0.99f);
	pObj->ClearAccumulator();

	//Set new data
	new_Projectile->SetPosition(pos);

	this->zDynamicProjectiles.push_back(new_Projectile);

	return true;
}

Actor* ActorHandler::GetActor(const long ID, const int TYPE) const
{
	Actor* ac = NULL;
	this->SearchForActor(ID, TYPE, &ac);

	return ac;
}

bool ActorHandler::RemovePlayerActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_PLAYER);

	if(index == -1 || (unsigned int)index >= this->zPlayers.size())
		return false;

	PlayerActor* temp = this->zPlayers[index];
	this->zPlayers.erase(this->zPlayers.begin() + index);
	this->zPhysicsEngine->DeletePhysicsObject(temp->GetPhysicObject());
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemovedDeadPlayerObject(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_DEAD_PLAYER);

	if(index == -1 || (unsigned int)index >= this->zDeadPlayers.size())
		return false;

	DeadPlayerObjectActor* temp = this->zDeadPlayers[index];

	this->zDeadPlayers.erase(this->zDeadPlayers.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveAnimalActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_ANIMAL);

	if(index == -1 || (unsigned int)index >= this->zAnimals.size())
		return false;
	
	AnimalActor* temp = this->zAnimals[index];
	this->zAnimals.erase(this->zAnimals.begin() + index);
	this->zPhysicsEngine->DeletePhysicsObject(temp->GetPhysicObject());
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveDeadAnimalObject( const long ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_DEAD_ANIMAL);

	if(index == -1 || (unsigned int)index >= this->zDeadPlayers.size())
		return false;

	DeadAnimalObjectActor* temp = this->zDeadAnimals[index];

	this->zDeadAnimals.erase(this->zDeadAnimals.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticFoodActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_FOOD);

	if(index == -1 || (unsigned int)index >= this->zFoods.size())
		return false;

	FoodObject* temp = this->zFoods[index];
	this->zFoods.erase(this->zFoods.begin() + index);
	
	SAFE_DELETE(temp);
	
	return true;
}

bool ActorHandler::RemoveStaticWeaponActor( const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_WEAPON);

	if(index == -1 || (unsigned int)index >= this->zWeapons.size())
		return false;

	WeaponObject* temp = this->zWeapons[index];

	this->zWeapons.erase(this->zWeapons.begin() + index);
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticContainerActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_CONTAINER);

	if(index == -1 || (unsigned int)index >= this->zContainers.size())
		return false;

	ContainerObject* temp = this->zContainers[index];

	this->zContainers.erase(this->zContainers.begin() + index);
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticProjectileActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_PROJECTILE);

	if(index == -1 || (unsigned int)index >= this->zStaticProjectiles.size())
		return false;

	StaticProjectileObject* temp = this->zStaticProjectiles[index];

	this->zStaticProjectiles.erase(this->zStaticProjectiles.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticMaterialActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_MATERIAL);

	if(index == -1 || (unsigned int)index >= this->zMaterials.size())
		return false;

	MaterialObject* temp = this->zMaterials[index];

	this->zMaterials.erase(this->zMaterials.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveDynamicProjectileActor(const long ID)
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE);

	if(index == -1 || (unsigned int)index >= this->zDynamicProjectiles.size())
		return false;

	DynamicProjectileObject* temp = this->zDynamicProjectiles[index];

	this->zDynamicProjectiles.erase(this->zDynamicProjectiles.begin() + index);
	this->zPhysicsEngine->DeletePhysicsObject(temp->GetPhysicObject());
	SAFE_DELETE(temp);

	return true;
}

const int ActorHandler::SearchForActor(const long ID, int TYPE) const
{
	if(TYPE == ACTOR_TYPE_PLAYER)
	{
		for (unsigned int it = 0; it < this->zPlayers.size(); it++)
		{
			if(this->zPlayers[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_DEAD_PLAYER)
	{
		for (unsigned int it = 0; it < this->zDeadPlayers.size(); it++)
		{
			if(this->zDeadPlayers[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zAnimals.size(); it++)
		{
			if(this->zAnimals[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_DEAD_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zDeadAnimals.size(); it++)
		{
			if(this->zDeadAnimals[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_FOOD)
	{
		for (unsigned int it = 0; it < this->zFoods.size(); it++)
		{
			if(this->zFoods[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_WEAPON)
	{
		for (unsigned int it = 0; it < this->zWeapons.size(); it++)
		{
			if(this->zWeapons[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_CONTAINER)
	{
		for (unsigned int it = 0; it < this->zContainers.size(); it++)
		{
			if(this->zContainers[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zStaticProjectiles.size(); it++)
		{
			if(this->zStaticProjectiles[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_MATERIAL)
	{
		for (unsigned int it = 0; it < this->zMaterials.size(); it++)
		{
			if(this->zMaterials[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zDynamicProjectiles.size(); it++)
		{
			if(this->zDynamicProjectiles[it]->GetID() == ID)
				return it;
		}
	}

	return -1;
}

const int ActorHandler::SearchForActor(const long ID, int TYPE, Actor** aOut) const
{
	if(TYPE == ACTOR_TYPE_PLAYER)
	{
		for (unsigned int it = 0; it < this->zPlayers.size(); it++)
		{
			if(this->zPlayers[it]->GetID() == ID)	
			{
				*aOut = this->zPlayers[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_DEAD_PLAYER)
	{
		for (unsigned int it = 0; it < this->zDeadPlayers.size(); it++)
		{
			if(this->zDeadPlayers[it]->GetID() == ID)	
			{
				*aOut = this->zDeadPlayers[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zAnimals.size(); it++)
		{
			if(this->zAnimals[it]->GetID() == ID)
			{
				*aOut = this->zAnimals[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_DEAD_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zDeadAnimals.size(); it++)
		{
			if(this->zDeadAnimals[it]->GetID() == ID)
			{
				*aOut = this->zDeadAnimals[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_FOOD)
	{
		for (unsigned int it = 0; it < this->zFoods.size(); it++)
		{
			if(this->zFoods[it]->GetID() == ID)
			{
				*aOut = this->zFoods[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_WEAPON)
	{
		for (unsigned int it = 0; it < this->zWeapons.size(); it++)
		{
			if(this->zWeapons[it]->GetID() == ID)
			{
				*aOut = this->zWeapons[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_CONTAINER)
	{
		for (unsigned int it = 0; it < this->zContainers.size(); it++)
		{
			if(this->zContainers[it]->GetID() == ID)
			{
				*aOut = this->zContainers[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zStaticProjectiles.size(); it++)
		{
			if(this->zStaticProjectiles[it]->GetID() == ID)
			{
				*aOut = this->zStaticProjectiles[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_MATERIAL)
	{
		for (unsigned int it = 0; it < this->zMaterials.size(); it++)
		{
			if(this->zMaterials[it]->GetID() == ID)
			{
				*aOut = this->zMaterials[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zDynamicProjectiles.size(); it++)
		{
			if(this->zDynamicProjectiles[it]->GetID() == ID)
			{
				*aOut = this->zDynamicProjectiles[it];
				return it;
			}
		}
	}

	aOut = NULL;
	return -1;
}

ObjectManager* ActorHandler::GetObjManager() const
{
	return this->zObjManager;
}

CollisionEvent ActorHandler::CheckMeeleCollision(BioActor* bActor, float range)
{
	unsigned int agressor_Type = 100;
	CollisionEvent cEvent = CollisionEvent();
	PhysicsCollisionData pcd;

	PhysicsObject* pOtherObj = NULL;

	agressor_Type = bActor->GetActorType();

	Vector3 view_Direction = bActor->GetDirection();

	Vector3 distance;
	Vector3 agressor_Victim_Direction;

	for (auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		if ((*it)->GetID() == bActor->GetID())
			continue;
		if (!(*it)->IsAlive())
			continue;

		pOtherObj = (*it)->GetPhysicObject();

		Vector3 position = bActor->GetPosition();
		position.y += 2.0f;
		Vector3 direction = bActor->GetDirection();

		Vector3 distance = pOtherObj->GetPosition() - position;
		distance.Normalize();

		pcd = this->zPhysicsEngine->GetCollisionRayMesh(position, direction, pOtherObj);
		
		if (pcd.collision && pcd.distance <= range)
		{
			cEvent.actor_aggressor_ID = bActor->GetID();
			cEvent.actor_aggressor_type = agressor_Type;

			cEvent.actor_victim_ID = (*it)->GetID();
			cEvent.actor_victim_type = ACTOR_TYPE_PLAYER;

			cEvent.event_type = MELEE_ATTACK;

			return cEvent;
		}
	}

	for (auto it = this->zAnimals.begin(); it < this->zAnimals.end(); it++)
	{
		if ((*it)->GetID() == bActor->GetID())
			continue;
		if (!(*it)->IsAlive())
			continue;

		pOtherObj = (*it)->GetPhysicObject();

		Vector3 position = bActor->GetPosition();
		position.y += 2.0f;
		Vector3 direction = bActor->GetDirection();

		pcd = this->zPhysicsEngine->GetCollisionRayMesh(position, direction, pOtherObj);

		if (pcd.collision && pcd.distance <= range)
		{
			cEvent.actor_aggressor_ID = bActor->GetID();
			cEvent.actor_aggressor_type = agressor_Type;

			cEvent.actor_victim_ID = (*it)->GetID();
			cEvent.actor_victim_type = ACTOR_TYPE_ANIMAL;

			cEvent.event_type = MELEE_ATTACK;

			return cEvent;
		}
	}
	return cEvent;
}

std::vector<CollisionEvent> ActorHandler::CheckProjectileCollisions()
{
	std::vector<CollisionEvent> collisionEvents;
	std::vector<BioActor*> pCollide;
	Vector3 position;
	float yValue;

	for (auto it = zDynamicProjectiles.begin(); it < zDynamicProjectiles.end(); it++)
	{
		position = (*it)->GetPosition();
		yValue = this->zWorld->CalcHeightAtWorldPos(Vector2(position.x, position.z));
		
		if (position.y <= yValue)
		{
			position.y = yValue;
			(*it)->SetPosition(position);
			(*it)->SetMoving(false);

			//No need to check this projectile
			continue;
		}

		DynamicActorVsBioActors((*it), zPlayers, pCollide);
		DynamicActorVsBioActors((*it), zAnimals, pCollide);

		if(!pCollide.empty())
		{
			bool stop = false;

			for (auto it_s = pCollide.begin(); it_s < pCollide.end() && !stop; it_s++)
			{
				if((*it_s)->TakeDamage((*it)->GetDamage()))
				{
					CollisionEvent ce;
					ce.actor_aggressor_ID = (*it)->GetObjPlayerOwner();
					ce.actor_aggressor_type = ACTOR_TYPE_PLAYER;
					ce.actor_victim_ID = (*it_s)->GetID();
					ce.actor_victim_type = (*it_s)->GetActorType();
					ce.event_type = BOW_ATTACK;
					collisionEvents.push_back(ce);

					//No need to continue, the player is dead
					stop = true;
				}

				(*it)->SetMoving(false);
			}
		}	
	}

	return collisionEvents;
}

void ActorHandler::CheckCollisions()
{

	std::vector<CollisionEvent> cEvent;

	/*PLAYERS VS PLAYERS*/
	std::vector<BioActor*> pCol;

	for(auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		if(!(*it)->HasMoved())
			continue;

		BioActorVSBioActors(*it, zPlayers, pCol);

		if(pCol.size() > 0)
			(*it)->RewindPosition();

		pCol.clear();
	}

	/*ANIMALS VS PLAYERS*/
	for(auto it = this->zAnimals.begin(); it < this->zAnimals.end(); it++)
	{
		if(!(*it)->HasMoved())
		continue;

		BioActorVSBioActors(*it, zPlayers, pCol);

		if(pCol.size() > 0)
			(*it)->RewindPosition();

		pCol.clear();
	}

}

void ActorHandler::BioActorVSBioActors(BioActor* pTest, std::vector<AnimalActor*> &actors, std::vector<BioActor*> &pCollide)
{
	PhysicsObject* pTemp = pTest->GetPhysicObject();
	Vector3 pPos = pTemp->GetPosition();

	for (auto it = actors.begin(); it < actors.end(); it++)
	{
		if((*it) == pTest)
			continue;


		Vector3 tPos = (*it)->GetPhysicObject()->GetPosition();
		float lenght = (pPos - tPos).GetLength();

		if(lenght <= MAX_COLLISION_DISTANCE_PLAYER)
		{
			(*it)->RewindPosition();
			pCollide.push_back(*it);
		}

	}
}

void ActorHandler::BioActorVSBioActors( BioActor* pTest, std::vector<PlayerActor*> &actors, std::vector<BioActor*> &pCollide )
{
	PhysicsObject* pTemp = pTest->GetPhysicObject();
	Vector3 pPos = pTemp->GetPosition();

	for (auto it = actors.begin(); it < actors.end(); it++)
	{
		if((*it) == pTest)
			continue;
		if(!(pTest)->HasMoved())
			continue;

		Vector3 tPos = (*it)->GetPhysicObject()->GetPosition();
		float lenght = (pPos - tPos).GetLength();

		if(lenght <= MAX_COLLISION_DISTANCE_PLAYER)
		{
			(*it)->RewindPosition();
			pCollide.push_back(*it);
		}

	}
}

void ActorHandler::DynamicActorVsBioActors( DynamicObjectActor* pTest, std::vector<PlayerActor*> &actors, std::vector<BioActor*> &pCollide)
{
	if(!pTest->IsMoving())
		return;

	float middle;
	Vector3 scale;
	Vector3 pos;
	Vector3 direction;
	PhysicsObject* mesh;

	scale = pTest->GetScale();
	middle = (pTest->GetModelLength() * max(max(scale.x, scale.y),scale.z)) / 2;
	mesh = pTest->GetPhysicObject();
	pos = mesh->GetPosition();
	direction = mesh->GetVelocity();
	direction.Normalize();

	PhysicsCollisionData pcd;

	for(auto it = actors.begin(); it < actors.end(); it++)
	{
		if((*it)->GetID() == pTest->GetObjPlayerOwner())
			continue;

		pcd = this->zPhysicsEngine->GetCollisionBoundingOnly(pos, direction, (*it)->GetPhysicObject());

		if(pcd.BoundingSphereCollision)
		{
			if((pcd.distance - (middle * 1.1f)) < 0.5f)
				pCollide.push_back(*it);
		}
	}
}

void ActorHandler::DynamicActorVsBioActors( DynamicObjectActor* pTest, std::vector<AnimalActor*> &actors, std::vector<BioActor*> &pCollide )
{
	if(!pTest->IsMoving())
		return;

	float middle;
	Vector3 scale;
	Vector3 pos;
	Vector3 direction;
	PhysicsObject* mesh;

	scale = pTest->GetScale();
	middle = (pTest->GetModelLength() * max(max(scale.x, scale.y),scale.z)) / 2;
	mesh = pTest->GetPhysicObject();
	pos = mesh->GetPosition();
	direction = mesh->GetVelocity();
	direction.Normalize();

	PhysicsCollisionData pcd;

	for(auto it = actors.begin(); it < actors.end(); it++)
	{
		if((*it)->GetID() == pTest->GetObjPlayerOwner())
			continue;

		pcd = this->zPhysicsEngine->GetCollisionBoundingOnly(pos, direction, (*it)->GetPhysicObject());

		if(pcd.BoundingSphereCollision)
		{
			if((pcd.distance - (middle * 1.1f)) < 0.5f)
				pCollide.push_back(*it);
		}
	}

}