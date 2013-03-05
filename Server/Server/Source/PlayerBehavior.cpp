#include <World/World.h>
#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"
#include "BioActor.h"
#include "WorldActor.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, Player* player) : 
	Behavior(actor, world)
{	
	this->zPlayer = player;
	this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
	this->zVelDown = 0.0f;

	this->zNearActorsIndex = 0;
	this->zNearByRadius = 100.0f;
	this->zCollisionRadius = 0.5f;
	this->zPlayerConfigReader = GetPlayerConfig();

// 	Vector3 centerPos;
// 	float humanRadius = 0.5f;//m
// 	float distanceInAxes;
// 	//Pythagoras theorem x2 = y2 + z2
// 	//both distances are the same, therefore x2 = y2 + y2 = 2y2
// 	//(distanceInAxes = y)
// 	distanceInAxes = sqrtf(powf(humanRadius, 2.0f) / 2); //use * 0.5f instead.
// 	//Build vectors going clockwise
// 
// 	Vector2 one = Vector2(distanceInAxes, distanceInAxes);//+X+Z
// 	Vector2 two = Vector2(distanceInAxes, -distanceInAxes);//+X-Z
// 	Vector2 one = Vector2(-distanceInAxes, -distanceInAxes);//-X-Z
// 	Vector2 one = Vector2(-distanceInAxes, distanceInAxes);//-X+Z

}

PlayerBehavior::~PlayerBehavior()
{
	this->zNearActors.clear();
}

bool PlayerBehavior::ProcessClientData( Vector3 direction, Vector4 rotation )
{
	this->zActor->SetDir(direction);
	this->zActor->SetRotation(rotation);

	return true;
}

bool PlayerBehavior::Update(float dt)
{
	float energy = zActor->GetEnergy();

	if (energy < 200.0f)

		energy += this->zPlayerConfigReader->GetVariable(ENERGY_COEFF) * dt;

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

	return CheckCollision(thisActorPosition, zNearBioActors);
}

Actor* PlayerBehavior::CheckWorldActorCollision()
{
	const Vector3 thisActorPosition = this->zActor->GetPosition();

	if( !this->zActor->CanCollide() )
		return NULL;

	if( !this->zWorld->IsBlockingAt(thisActorPosition.GetXZ()) )
		return NULL;

	return CheckCollision(thisActorPosition, zNearWorldActors);
}

Actor* PlayerBehavior::CheckCollision(const Vector3& pos, const std::set<Actor*>& actors )
{
	auto it_end = actors.end();
	for(auto it = actors.begin(); it != it_end; it++)
	{
		float distance = ( pos - (*it)->GetPosition() ).GetLength();
		if(distance <= 1.0f)
			return (*it);
	}

	return NULL;
}

void PlayerBehavior::SetNearActors( std::set<Actor*> actors )
{
	this->zNearActors = actors;
	this->zNearActors.erase(this->zActor);

	auto it_end = zNearActors.end();
	for (auto it = this->zNearActors.begin(); it != it_end; it++)
	{
		if( dynamic_cast<WorldActor*>(*it) )
		{
			this->zNearWorldActors.insert(*it);
		}
		else if ( dynamic_cast<BioActor*>(*it) )
		{
			this->zNearBioActors.insert(*it);
		}
	}
}
