#include "AnimalActor.h"


AnimalActor::AnimalActor( bool genID /*= true*/ ) : BioActor()
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();

}

AnimalActor::AnimalActor( const Vector3& startPos, bool genID /*= true*/ ) : BioActor(startPos)
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();
}

AnimalActor::AnimalActor( const Vector3& startPos, const Vector4& rot, bool genID /*= true*/ ) : BioActor(startPos, rot)
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();
}

AnimalActor::~AnimalActor()
{
	zTargets.clear();
}

void AnimalActor::Update( float deltaTime )
{

}

void AnimalActor::InitPathfinder()
{
	//this->zPathfinder.InitAI(,5); //How to send in the bitset might be a bit of a hastle, oh well.
}

Behaviour AnimalActor::GetBehaviour()
{
	return this->zMentalState;
}

void AnimalActor::SetBehaviour(Behaviour newBehaviour)
{
	this->zMentalState = newBehaviour;
}

bool AnimalActor::GetIfNeedPath()
{
	return this->zNeedPath;
}

void AnimalActor::SetIfNeedPath(bool needPath)
{
	this->zNeedPath = needPath;
}

float AnimalActor::GetAlertnessLevel()
{
	return this->zAlertnessLevel;
}

void AnimalActor::SetAlertnessLevel(float alertness)
{
	this->zAlertnessLevel = alertness;
	if(this->zAlertnessLevel > this->zAlertnessMax)
	{
		this->zAlertnessLevel = this->zAlertnessMax;
	}
	else if(this->zAlertnessLevel < 0)
	{
		this->zAlertnessLevel = 0;
	}
}

float AnimalActor::GetAlertnessMax()
{
	return this->zAlertnessMax;
}

void AnimalActor::SetAlertnessMax(float max)
{
	this->zAlertnessMax = max;
}

float AnimalActor::GetFearLevel()
{
	return this->zFearLevel;
}

void AnimalActor::SetFearLevel(float fear)
{
	this->zFearLevel = fear;
	if(this->zFearLevel > this->zFearMax)
	{
		this->zFearLevel = this->zFearMax;
	}
	else if(this->zFearLevel < 0)
	{
		this->zFearLevel = 0;
	}
}

float AnimalActor::GetFearMax()
{
	return this->zFearMax;
}

void AnimalActor::SetFearMax(float max)
{
	this->zFearMax = max;
}

int AnimalActor::GetLastDistanceCheck()
{
	return this->zLastDistanceCheck;
}

void AnimalActor::SetLastDistanceCheck(int distance)
{
	this->zLastDistanceCheck = distance;
}
