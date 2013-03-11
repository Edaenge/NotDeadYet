#include <World/World.h>
#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"
#include "BioActor.h"
#include "WorldActor.h"
#include "ActorSynchronizer.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, Player* player) : 
	Behavior(actor, world)
{	
	this->zPlayer = player;
	this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
	this->zVelDown = 0.0f;

	this->zNearActorsIndex = 0;
	this->zNearByRadius = 100.0f;

	// Bigger Anchor For Players
	zAnchor->radius = 50.0f;
}

PlayerBehavior::~PlayerBehavior()
{
	this->zNearActors.clear();
	this->zNearBioActors.clear();
	this->zNearWorldActors.clear();
}

bool PlayerBehavior::ProcessClientData( Vector3 direction, Vector4 rotation )
{
	this->zActor->SetDir(direction);
	this->zActor->SetRotation(rotation);

	return true;
}

bool PlayerBehavior::Update(float dt)
{
	static PlayerConfigReader* playerConfigReader = GetPlayerConfig();
	float energy = zActor->GetEnergy();

	if (energy < 200.0f)

		energy += playerConfigReader->GetVariable(ENERGY_COEFF) * dt;

	if (energy >= 200.0f)
		energy = 200.0f;

	zActor->SetEnergy(energy);

	return false;
}

bool PlayerBehavior::RefreshNearCollideableActors( const std::set<Actor*>& actors )
{
	bool canCollide = this->zActor->CanCollide();

	if(!canCollide)
		return false;

	unsigned int size = actors.size();
	// Increment 5%
	unsigned int increment = (unsigned int)(size * 0.05);

	Vector3 pos = this->zActor->GetPosition();

	if(zNearActorsIndex >= size)
		zNearActorsIndex = 0;

	auto it = actors.begin();
	std::advance(it, zNearActorsIndex);

	//Check if the new actors is within range.
	for (unsigned int i = 0; i < increment; i++)
	{

		if( it == actors.end() )
		{
			zNearActorsIndex = 0;
			it = actors.begin();
		}

		if( (*it) != this->zActor && (*it)->CanCollide() )
		{
			Vector3 vec = (*it)->GetPosition() - pos;
			BioActor* bioActor = dynamic_cast<BioActor*>(*it);
			WorldActor* worldActor = dynamic_cast<WorldActor*>(*it);

			auto found = this->zNearActors.find(*it);

			if( vec.GetLength() <= zNearByRadius )
			{
				if( found == zNearActors.end() )
					zNearActors.insert(*it);

				if(bioActor && zNearBioActors.find(*it) == zNearBioActors.end())
					this->zNearBioActors.insert(*it);

				else if(worldActor && zNearWorldActors.find(*it) == zNearWorldActors.end())
					this->zNearWorldActors.insert(*it);
			}
			else
			{
				if( found != zNearActors.end() )
					zNearActors.erase(*it);

				if( bioActor && zNearBioActors.find(*it) != zNearBioActors.end() )
					this->zNearBioActors.erase(*it);

				else if( worldActor && zNearWorldActors.find(*it) != zNearWorldActors.end() )
					this->zNearWorldActors.erase(*it);
			}
		}

		zNearActorsIndex++;
		it++;
	}

	return true;
}

Actor* PlayerBehavior::CheckBioActorCollision()
{
	const Vector3 thisActorPosition = this->zActor->GetPosition();

	if( !this->zActor->CanCollide() )
		return NULL;

	return CheckCollision(thisActorPosition, this->zActor->GetCollisionRadius(), zNearBioActors);
}

Actor* PlayerBehavior::CheckWorldActorCollision()
{
	if( !this->zActor )
		return NULL;

	if( !this->zActor->CanCollide() )
		return NULL;
	
	const Vector3 thisActorPosition = this->zActor->GetPosition();
	const Vector2 thisActorPositionXZ = thisActorPosition.GetXZ();

	//Check all collision points
	const Vector2* collidePoints = this->zActor->GetCollisionPoints();
	Vector2 blockedPos;
	bool isBlocking = false;

	for( int i = 0; i < 4 && !isBlocking; i++)
	{
		blockedPos = thisActorPositionXZ + collidePoints[i];
		if( this->zWorld->IsBlockingAt(blockedPos) )
			isBlocking = true;
	}

	if( !isBlocking )
		return NULL;

	return CheckCollision(thisActorPosition, this->zActor->GetCollisionRadius(), zNearWorldActors);
}

Actor* PlayerBehavior::CheckCollision(const Vector3& pos, const float& radius, const std::set<Actor*>& actors )
{
	auto it_end = actors.end();
	for(auto it = actors.begin(); it != it_end; it++)
	{
		float distance = ( pos - (*it)->GetPosition() ).GetLength();
		if( distance < radius + (*it)->GetCollisionRadius() )
			return (*it);
	}

	return NULL;
}

void PlayerBehavior::OnEvent( Event* e )
{
	Behavior::OnEvent(e);

	if( ActorRemoved* AR = dynamic_cast<ActorRemoved*>(e) )
	{
		auto found = this->zNearActors.find(AR->zActor);
		if(found != this->zNearActors.end())
			this->zNearActors.erase(found);

		found = this->zNearBioActors.find(AR->zActor);
		if( found != this->zNearBioActors.end() )
			this->zNearBioActors.erase(found);
		else
		{
			found = this->zNearWorldActors.find(AR->zActor);
			if( found != this->zNearWorldActors.end() )
				this->zNearWorldActors.erase(found);
		}
	}
}