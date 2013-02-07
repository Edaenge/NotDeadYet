#include "BioActor.h"


BioActor::BioActor() : Actor()
{
	this->zState = STATE_IDLE;
	this->zVelocity = V_WALK_SPEED;

	this->zAlive = true;
	this->zHealthMax = 100;
	this->zHealth = zHealthMax;
	this->zHealthChanged = true;

	this->zStaminaMax = 100;
	this->zStamina = zStaminaMax;
	this->zStaminaCof = 0.10f;
	this->zStaminaChanged = true;
}

BioActor::~BioActor()
{
}


bool BioActor::TakeDamage(const Damage& dmg, Actor* dealer)
{
	this->zHealth -= dmg.GetTotal();
	this->zHealthChanged = true;

	if(this->zHealth <= 0.0f)
	{
		this->zHealth = 0.0f;
		this->zAlive = false;
	}

	// Notify Damage
	BioActorTakeDamageEvent BATD;
	BATD.zActor = this;
	BATD.zDamage = dmg;
	BATD.zDealer = dealer;
	NotifyObservers(&BATD);

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
	
	this->zStaminaChanged = true;

	return true;
}

void BioActor::RewindPosition()
{
	SetPosition(zPreviousPos);
}

bool BioActor::HasMoved()
{
	Vector3 curPos = GetPosition();

	if(curPos == this->zPreviousPos)
		return false;	
	return true;
}

std::string BioActor::ToMessageString( NetworkMessageConverter* NMC )
{
	string msg = "";
	msg += NMC->Convert(MESSAGE_TYPE_STATE, this->zState);

	if(zHealthChanged)
	{
		msg += NMC->Convert(MESSAGE_TYPE_HEALTH, this->zHealth);
		this->zHealthChanged = false;
	}

	if(zStaminaChanged)
	{
		msg += NMC->Convert(MESSAGE_TYPE_STAMINA, this->zStamina);
		this->zStaminaChanged = false;
	}

	return msg;
}
