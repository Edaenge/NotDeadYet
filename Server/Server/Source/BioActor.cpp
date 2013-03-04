#include "BioActor.h"
#include "Physics.h"

BioActor::BioActor() : Actor()
{
	this->zPlayer = NULL;

	zInventory = new Inventory();
	this->zState = STATE_IDLE;
	this->zVelocity = V_WALK_SPEED;

	this->zAlive = true;
	this->zHealthMax = 100;
	this->zHealth = zHealthMax;
	this->zHealthChanged = true;

	this->zStaminaMax = 100;
	this->zStamina = zStaminaMax;
	this->zStaminaCof = 2.2f;
	this->zStaminaChanged = true;

	this->zBleedingLevel = 0;
	this->zValid = false;
}

BioActor::BioActor(Player* player) : Actor()
{
	this->zPlayer = player;

	zInventory = new Inventory();
	this->zState = STATE_IDLE;
	this->zVelocity = V_WALK_SPEED;

	this->zAlive = true;
	this->zHealthMax = 100;
	this->zHealth = zHealthMax;
	this->zHealthChanged = true;

	this->zStaminaMax = 100;
	this->zStamina = zStaminaMax;
	this->zStaminaCof = 2.2f;
	this->zStaminaChanged = true;

	this->zBleedingLevel = 0;
	this->zValid = false;
}

BioActor::~BioActor()
{
	SAFE_DELETE(this->zInventory);
}

bool BioActor::TakeDamage(Damage& dmg, Actor* dealer)
{
	// Notify Damage
	BioActorTakeDamageEvent BATD;
	BATD.zActor = this;
	BATD.zDamage = &dmg;
	BATD.zDealer = dealer;
	NotifyObservers(&BATD);

	this->zHealth -= dmg.GetTotal();
	this->zHealthChanged = true;

	if(this->zHealth <= 0.0f)
	{
		this->zAlive = false;
		this->zHealth = 0.0f;
	}

	if(dmg.GetTotal() / this->zHealth > 0.20 && dmg.GetBleedFactor() > 0.6)
	{
		float currentBleeding = this->GetBleeding();
		this->SetBleeding( currentBleeding + 1 );
	}

	if(!zAlive)
	{
		//RotateMesh
		Vector3 up = Vector3(0, 1, 0);
		Vector3 forward = this->GetDir();
		forward.y = 0;
		forward.Normalize();
		Vector3 around = up.GetCrossProduct(forward);
		around.Normalize();
		float angle = 3.14f * 0.5f;
		
		PhysicsObject* pObject = this->GetPhysicsObject();
		if (pObject)
		{
			this->GetPhysicsObject()->SetQuaternion(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			this->SetRotation(around,angle);

			GetPhysics()->DeletePhysicsObject(pObject);
			this->SetPhysicsObject(NULL);
		}
	}
	
	return this->zAlive;
}

void BioActor::Kill()
{
	Damage dmg;
	dmg.fallingDamage = zHealth;

	TakeDamage(dmg, this);
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

bool BioActor::HasMoved()
{
	Vector3 curPos = GetPosition();

	if(curPos == GetPreviousPos())
		return false;	

	return true;
}

std::string BioActor::ToMessageString( NetworkMessageConverter* NMC )
{
	string msg = "";
	msg += NMC->Convert(MESSAGE_TYPE_STATE, (float)this->zState);

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

void BioActor::SetState( const int state )
{
	if (state != this->zState)
	{
		this->zState = state;

		BioActorStateEvent BASE;
		BASE.zBioActor = this;
		NotifyObservers(&BASE);
	}
}
