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
	this->zPlayerConfigReader = GetPlayerConfig();
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
		energy += this->zPlayerConfigReader->GetVariable(ENERGY_COEFF) * dt;

	if (energy >= 200.0f)
		energy = 200.0f;

	zActor->SetEnergy(energy);

	return false;
}