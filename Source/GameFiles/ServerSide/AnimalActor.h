/*
Made by Edänge Simon 2012-12-11
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/BioActor.h"

/*This is an animal. It inherits from BioActor.*/
class AnimalActor : public BioActor
{
public:
	AnimalActor(const int ID);
	AnimalActor(const int ID, const Vector3& startPos);
	AnimalActor(const int ID, const Vector3& startPos, const Vector4& rot);
	virtual ~AnimalActor();

	virtual void Update(float deltaTime);



private:
};
