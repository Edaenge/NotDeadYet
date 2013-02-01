#include "AnimalActor.h"


AnimalActor::AnimalActor( bool genID /*= true*/ ) : BioActor()
{
	if(genID)
		this->zID = this->GenerateID();
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
		this->zID = this->GenerateID();
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
		this->zID = this->GenerateID();
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

//void AnimalActor::Update( float deltaTime )
//{
//
//}
//
//void AnimalActor::UpdateForAnimal( float deltaTime )
//{
//
//}
//
//void AnimalActor::UpdateForPlayer( float deltaTime )
//{
//
//}

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

float AnimalActor::GetLastDistanceCheck()
{
	return this->zLastDistanceCheck;
}

void AnimalActor::SetLastDistanceCheck(float distance)
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

Vector3 AnimalActor::ExaminePathfindingArea()
{
		Vector3 dest;
		//We make a check to see if the position is available, to avoid wasting time in the pathfinder functions.
		dest = this->zMainTarget.position - this->GetPosition();
		dest.Normalize();
		dest = dest * -1;
		dest *= (float)this->zFleeDistance;

		dest = this->GetPosition() + dest;
				
		bool getEmergencyDirection = false;
		bool foundPath = false;
		int counter = 0; //Just for testing stuff
		getEmergencyDirection = false;


		while(foundPath == false)
		{
			getEmergencyDirection = false;
			if(dest.x < 0)
			{
				dest.x = 0;
			}
			else if(dest.x > this->zWorld->GetWorldSize().x)
			{
				dest.x = this->zWorld->GetWorldSize().x - 1; 
			}

			if(dest.z < 0)
			{
				dest.z = 0;
			}
			else if(dest.z > this->zWorld->GetWorldSize().y)
			{
				dest.z = this->zWorld->GetWorldSize().y - 1;
			}
			while(this->zWorld->IsBlockingAt(Vector2(dest.x, dest.z)) && ( dest - this->GetPosition() ).GetLength() > 2)
			{
				dest = (dest - this->GetPosition());
				dest = this->GetPosition() + (dest - dest * 0.75);
						
			}
			if(( dest - this->GetPosition() ).GetLength() < 2)
			{
				getEmergencyDirection = true; //There is basically a thick wall in the way...
			}

			if(getEmergencyDirection) //...so let's find another direction.
			{
				//this->zDestination.Normalize();
				counter++;
				float angle = (-10 * counter) * 3.14/180;
				float oldX, oldY;
				oldX = dest.x;
				oldY = dest.z;

				dest.x = cos(angle) * oldX - sin(angle) * oldY;
				dest.z = cos(angle) * oldY + sin(angle) * oldX;
				dest.Normalize();
				dest = dest * zFleeDistance;

					
			}
			else
			{
				foundPath = true;
				return dest;
			}
		}
}

std::string AnimalActor::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg;
	msg = BioActor::ToMessageString(NMC);

	return msg;
}
