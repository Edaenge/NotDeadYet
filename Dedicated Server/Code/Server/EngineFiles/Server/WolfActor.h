
#pragma once

#include "AnimalActor.h"



/*This is a Wolf. It inherits from AnimalActor.*/
class WolfActor : public AnimalActor
{
public:
	WolfActor(bool genID = true, bool isPlayerControlled = false);
	WolfActor(const Vector3& startPos, PhysicsObject* pObj, bool genID = true, bool isPlayerControlled = false);
	WolfActor(const Vector3& startPos, PhysicsObject* pObj, const Vector4& rot, bool genID = true, bool isPlayerControlled = false);
	virtual ~WolfActor();

	void InitWolf() throw(...);
	virtual void Update(float deltaTime);
	virtual void UpdateForAnimal(float deltaTime);
	virtual void UpdateForPlayer(float deltaTime);
	virtual void LargeSuddenSoundReaction();


private:
	
};
