
#pragma once

#include "AnimalActor.h"



/*This is a deer. It inherits from AnimalActor.*/
class DeerActor : public AnimalActor
{
public:
	DeerActor(PhysicsObject* pObj, bool genID = true, bool isPlayerControlled = false);
	virtual ~DeerActor();

	void InitDeer() throw(...);
	virtual void Update(float deltaTime);
	virtual void UpdateForAnimal(float deltaTime);
	virtual void UpdateForPlayer(float deltaTime);
	virtual void LargeSuddenSoundReaction();


private:
	
};
