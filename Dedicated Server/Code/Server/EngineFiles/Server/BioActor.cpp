#include "BioActor.h"


BioActor::BioActor() : PhysicsActor()
{
	InitValues();
}

BioActor::BioActor(const Vector3& startPos, PhysicsObject* pObj) : PhysicsActor(startPos, pObj)
{
	InitValues();
}

BioActor::BioActor(const Vector3& startPos, PhysicsObject* pObj, const Vector4& rot) : PhysicsActor(startPos, pObj, rot)
{
	InitValues();
}

BioActor::~BioActor()
{
}

void BioActor::InitValues()
{
	if(zPhysicObj)
		this->zPreviousPos = this->zPhysicObj->GetPosition();
	
	this->zState = STATE_IDLE;
	this->zVelocity = V_WALK_SPEED;
	this->zActorModel = "none";

	this->zAlive = true;
	this->zHealthMax = 100;
	this->zHealth = zHealthMax;
	this->zHealthChanged = false;
	
	this->zStaminaMax = 100;
	this->zStamina = zStaminaMax;
	this->zStaminaCof = 0.10f;
	this->zStaminaChanged = false;
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

void BioActor::RewindPosition()
{
	SetPosition(zPreviousPos);
}

bool BioActor::HasMoved()
{
	if(GetPosition() == this->zPreviousPos)
		return false;
	
	return true;
}

void BioActor::AddChangedHData(string& mess, NetworkMessageConverter* nmc)
{
	if(zHealthChanged)
	{
		mess += nmc->Convert(MESSAGE_TYPE_HEALTH, this->zHealth);
		this->zHealthChanged = false;
	}
	if(zStaminaChanged)
	{
		mess += nmc->Convert(MESSAGE_TYPE_STAMINA, this->zStamina);
		this->zStaminaChanged = false;
	}
}