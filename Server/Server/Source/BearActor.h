
#pragma once

#include "AnimalActor.h"



/*This is a Bear. It inherits from AnimalActor.*/
class BearActor : public AnimalActor
{
public:
	BearActor(bool genID = true, bool isPlayerControlled = false);
	BearActor(const Vector3& startPos, PhysicsObject* pObj, bool genID = true, bool isPlayerControlled = false);
	BearActor(const Vector3& startPos, PhysicsObject* pObj, const Vector4& rot, bool genID = true, bool isPlayerControlled = false);
	virtual ~BearActor();

	void InitBear() throw(...);
	virtual void Update(float deltaTime);
	virtual void UpdateForAnimal(float deltaTime);
	virtual void UpdateForPlayer(float deltaTime);
	virtual void LargeSuddenSoundReaction();


private:

	float zCalmToAggressiveThreshold;
	
};
