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

	this->zNearByRadius = 100.0f;

	// Bigger Anchor For Players
	zAnchor->radius = 50.0f;
}

PlayerBehavior::~PlayerBehavior()
{
	this->zNearActors.clear();
	this->zNearDynamicActors.clear();
	this->zNearStaticActors.clear();
}

bool PlayerBehavior::ProcessClientData( const Vector3& direction, const Vector4& rotation )
{
	this->zActor->SetDir(direction);
	this->zActor->SetRotation(rotation);

	return true;
}

bool PlayerBehavior::Update(float dt)
{
	static const float energy_Coeff = GetPlayerConfig()->GetVariable(ENERGY_COEFF);
	float energy = zActor->GetEnergy();

	if (energy < 200.0f)

		energy += energy_Coeff * dt;

	if (energy >= 200.0f)
		energy = 200.0f;

	zActor->SetEnergy(energy);

	return false;
}