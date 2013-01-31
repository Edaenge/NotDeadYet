#include "AnimalActor.h"


AnimalActor::AnimalActor( bool genID /*= true*/ ) : BioActor()
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();
	
	Target vectorFilling; 
	vectorFilling.health = 0;
	vectorFilling.movementNoise = 0;
	vectorFilling.position = Vector3(0,0,0);
	vectorFilling.valid = false;
	zActorType = ACTOR_TYPE_ANIMAL;

	for(int i = 0; i < 32; i++)
	{
	this->zTargets.push_back(vectorFilling);
	}

	this->zIntervalCounter = 0;
}

AnimalActor::AnimalActor( const Vector3& startPos,  PhysicsObject* pObj, bool genID /*= true*/ ) : BioActor(startPos, pObj)
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();

	Target vectorFilling; 
	vectorFilling.health = 0;
	vectorFilling.movementNoise = 0;
	vectorFilling.position = Vector3(0,0,0);
	vectorFilling.valid = false;
	zActorType = ACTOR_TYPE_ANIMAL;

	for(int i = 0; i < 32; i++)
	{
	this->zTargets.push_back(vectorFilling);
	}

	this->zIntervalCounter = 0;

	
}

AnimalActor::AnimalActor( const Vector3& startPos,  PhysicsObject* pObj, const Vector4& rot, bool genID /*= true*/ ) : BioActor(startPos, pObj, rot)
{
	if(genID)
		this->GenerateID();
	this->InitPathfinder();

	Target vectorFilling; 
	vectorFilling.health = 0;
	vectorFilling.movementNoise = 0;
	vectorFilling.position = Vector3(0,0,0);
	vectorFilling.valid = false;

	for(int i = 0; i < 32; i++)
	{
	this->zTargets.push_back(vectorFilling);
	}

	this->zIntervalCounter = 0;

	
}

AnimalActor::~AnimalActor()
{
	zTargets.clear();
}

void AnimalActor::Update( float deltaTime )
{

}

void AnimalActor::UpdateForAnimal( float deltaTime )
{

}

void AnimalActor::UpdateForPlayer( float deltaTime )
{

}

void AnimalActor::InitPathfinder()
{
	this->zPathfinder.InitAI(0.5,95);
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

//float AnimalActor::GetAlertnessLevel()
//{
//	return this->zAlertnessLevel;
//}
//
//void AnimalActor::SetAlertnessLevel(float alertness)
//{
//	this->zAlertnessLevel = alertness;
//	if(this->zAlertnessLevel > this->zAlertnessMax)
//	{
//		this->zAlertnessLevel = this->zAlertnessMax;
//	}
//	else if(this->zAlertnessLevel < 0)
//	{
//		this->zAlertnessLevel = 0;
//	}
//}
//
//float AnimalActor::GetAlertnessMax()
//{
//	return this->zAlertnessMax;
//}
//
//void AnimalActor::SetAlertnessMax(float max)
//{
//	this->zAlertnessMax = max;
//}

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

void AnimalActor::LargeSuddenSoundReaction()
{
	this->SetFearLevel(this->GetFearLevel());
}

std::vector<Vector2>& AnimalActor::GetPath()
{
	return this->zCurrentPath;
}

void AnimalActor::SetTargetInfo(int number, Vector3 pos, float velocity, float health, Type kind)
{
	this->zTargets[number].position = pos;
	this->zTargets[number].movementNoise = velocity;
	this->zTargets[number].health = health;
	this->zTargets[number].kind = kind;
}

int AnimalActor::GetCurrentTargets()
{
	return this->zCurrentNrOfTargets;
}

void AnimalActor::SetCurrentTargets(int number)
{
	this->zCurrentNrOfTargets = number;
}

float AnimalActor::GetPreviousHealth()
{
	return this->zPreviousHealth;
}

void AnimalActor::SetPreviousHealth(float oldHealth)
{
	this->zPreviousHealth = oldHealth;
}

bool AnimalActor::GetIfPlayerControlled()
{
	return this->zIsPlayerControlled;
}

void AnimalActor::SetIfPlayerControlled(bool isControlled)
{
	this->zIsPlayerControlled = isControlled;
}

Type AnimalActor::GetType()
{
	return this->zType;
}

void AnimalActor::SetType(Type theType)
{
	this->zType = theType;
}

void AnimalActor::SetWorldPointer(World* theWorld)
{
	this->zWorld = theWorld;
	this->zPathfinder.SetWorldPointer(this->zWorld);
	
}

std::string AnimalActor::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg;
	msg = BioActor::ToMessageString(NMC);

	return msg;
}
