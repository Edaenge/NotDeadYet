#include <World/World.h>
#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"
#include "PlayerConfiguration.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, Player* player) : 
	Behavior(actor, world)
{	
	this->zPlayer = player;
	this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
	this->zVelDown = 0.0f;
}

PlayerBehavior::~PlayerBehavior()
{

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
		energy += GetPlayerConfiguration().zEnergyCoeff * dt;

	if (energy >= 200.0f)
		energy = 200.0f;

	zActor->SetEnergy(energy);

	return false;
}