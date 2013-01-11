
#pragma once

#include "AnimalActor.h"
#include <fstream>


/*This is a deer. It inherits from AnimalActor.*/
class DeerActor : public AnimalActor
{
public:
	DeerActor(bool genID = true);
	DeerActor(const Vector3& startPos, bool genID = true);
	DeerActor(const Vector3& startPos, const Vector4& rot, bool genID = true);
	virtual ~DeerActor();

	void InitDeer() throw(...);
	virtual void Update(float deltaTime);


private:
	
	//string test;
};
