#include <World/World.h>
#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, Player* player) : 
	Behavior(actor, world)
{	
	this->zPlayer = player;
	this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
	this->zVelDown = 0.0f;

	this->zNearActorsIndex = 0;
	this->zCollisionRadius = 30.0f;
	this->zPlayerConfigReader = GetPlayerConfig();}

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
	// Increment 10%
	unsigned int increment = size * 0.1;
	unsigned int counter = 0;

	Vector3 pos = this->zActor->GetPosition();

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
			auto found = zNearActors.find(*it);

			if( found == zNearActors.end() && vec.GetLength() <= zCollisionRadius )
			{
				counter++;
				zNearActors.insert(*it);
			}
		}

		zNearActorsIndex++;
		it++;
	}

	//Remove old actors that is not nearBy
	auto begin = zNearActors.begin();
	std::advance(begin, counter);

	while( begin != zNearActors.end() )
	{
		if(!(*begin))
		{
			begin = zNearActors.erase(begin);
			continue;
		}

		Vector3 vec = (*begin)->GetPosition() - pos;
		if( vec.GetLength() > zCollisionRadius)
			begin = zNearActors.erase(begin);
		else
		{
			begin++;
		}
	}

	return true;
}

Actor* PlayerBehavior::CheckCollision()
{
	if( !this->zActor->CanCollide() )
		return NULL;

	for(auto it = this->zNearActors.begin(); it != this->zNearActors.end(); it++)
	{
		float distance = (this->zActor->GetPosition() - (*it)->GetPosition()).GetLength();
		if(distance <= 1.0f)
			return (*it);
	}

	return NULL;
}

void PlayerBehavior::SetNearActors( std::set<Actor*> actors )
{
	this->zNearActors = actors;
	this->zNearActors.erase(this->zActor);
}
