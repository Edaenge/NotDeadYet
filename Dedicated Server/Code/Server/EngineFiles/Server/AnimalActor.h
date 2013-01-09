/*
Made by Ed�nge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska h�gskola.
*/

#pragma once

#include "BioActor.h"

/*This is a NPC animal. It inherits from BioActor.*/
/*ID is generated in Actor.h*/
class AnimalActor : public BioActor
{
public:
	AnimalActor(bool genID = true);
	AnimalActor(const Vector3& startPos, bool genID = true);
	AnimalActor(const Vector3& startPos, const Vector4& rot, bool genID = true);
	virtual ~AnimalActor();

	virtual void Update(float deltaTime);



private:
};
