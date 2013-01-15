#include "BioActor.h"


BioActor::BioActor() : Actor()
{
	InitValues();
}

BioActor::BioActor(const Vector3& startPos) : Actor(startPos)
{
	InitValues();
}

BioActor::BioActor(const Vector3& startPos, const Vector4& rot) : Actor(startPos, rot)
{
	InitValues();
}

BioActor::~BioActor()
{
}

void BioActor::InitValues()
{
	this->zPreviousPos = this->zPos;
	this->zState = STATE_IDLE;
	this->zVelocity = V_WALK_SPEED;
	this->zDir = Vector3(1,0,0);
	this->zUp = Vector3(0,1,0);
	this->zActorModel = "none";

	this->zAlive = true;
	this->zHealthMax = 100;
	this->zHealth = zHealthMax;

	this->zStaminaMax = 100;
	this->zStamina = zStaminaMax;
	this->zStaminaCof = 0.10f;
	zPhysicObj = NULL;
}

bool BioActor::TakeDamage( const float dmg )
{
	this->zHealth -= dmg; 
	
	if(zHealth <= 0.0f)
		this->zAlive = false;

	return this->zAlive;
}

bool BioActor::IsAlive() const
{
	return this->zAlive;
}

bool BioActor::Sprint(float dt)
{

	float temp = this->zStamina;
	temp -= dt * this->zStaminaCof;

	if(temp < 0.0f)
		return false;
	else
		this->zStamina = temp;
	


	return true;
}


	

