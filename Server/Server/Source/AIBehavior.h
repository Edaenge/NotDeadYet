//Simon Edänge 2013-02-24

#pragma once

#include <World/World.h>
#include "Behavior.h"
#include "Actor.h"
#include "AI.h"

enum MentalState
{
	IDLE, 
	CALM, 
	SUSPICIOUS,
	AGGRESSIVE, 
	AFRAID
};

enum Type
{
	HUMAN,
	DEER,
	WOLF,
	BEAR
};

struct Target //Supposed to represent an entity near the animal, like a player, or possibly other animal (removed until deemed necessary)
{
	Vector3 position;
	//float	threatLevel;
	float	health;
	float	movementNoise; //The velocity of movement, currently used here to determine how much an animal should hear.
	bool	valid;
	Type	kind;
};

class AIBehavior : public Behavior
{
public:
	AIBehavior(Actor* actor, World* world);
	virtual ~AIBehavior();
protected:
	
private:
};