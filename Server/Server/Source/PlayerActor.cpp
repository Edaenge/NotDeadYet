#include "PlayerActor.h"
#include "ClientServerMessages.h"
#include "MaloWFileDebug.h"

PlayerActor::PlayerActor( Player* player, PhysicsObject* physObj ) 
	: BioActor(player)
{
	SetPhysicsObject(physObj);
	/*InitValues();*/

	this->zFullness = 100.0f;
	this->zHydration = 100.0f;

	this->zFullnessMax = 100;
	this->zHydrationMax = 100;
	this->zBreathSoundTimer = 1.5f;
	this->zType = ACTOR_TYPE_PLAYER;

}

PlayerActor::~PlayerActor()
{
}

Item* PlayerActor::DropItem(const long ID)
{
	Item* item = this->zInventory->SearchAndGetItem(ID);

	if(!item)
	{
		MaloW::Debug("Failed Item=NULL ID: " + MaloW::convertNrToString((float)ID));
		return NULL;
	}
	
	RangedWeapon* rwp = this->zInventory->GetRangedWeapon();
	MeleeWeapon* mwp = this->zInventory->GetMeleeWeapon();
	Projectile* proj = this->zInventory->GetProjectile();

	if(rwp && dynamic_cast<RangedWeapon*>(item) == rwp)
	{
		this->zInventory->UnEquipRangedWeapon();
	}
	else if(mwp && dynamic_cast<MeleeWeapon*>(item) == mwp)
	{
		this->zInventory->UnEquipMeleeWeapon();
	}
	else if(proj && dynamic_cast<Projectile*>(item) == proj)
	{
		this->zInventory->UnEquipProjectile();
	}

	item = this->zInventory->RemoveItem(item);

	if (item && Messages::FileWrite())	
		Messages::Debug("Removed successes: " + MaloW::convertNrToString((float)ID));

	return item;
}

float PlayerActor::GetFullness()
{
	return this->zFullness;
}

float PlayerActor::GetHydration()
{
	return this->zHydration;
}

void PlayerActor::SetFullness(float fullness, const bool notify /*notify = true*/)
{
	if( fullness != this->zFullness )
	{
		this->zFullness = fullness;

		if(notify)
		{
			PlayerActorPhysicalConditionHungerEvent e;
			e.zPlayerActor = this;
			NotifyObservers(&e);
		}
	}
}

void PlayerActor::SetHydration(float hydration, const bool notify /*notify = true*/)
{
	if(this->zHydration != hydration)
	{
		this->zHydration = hydration;

		if(notify)
		{
			PlayerActorPhysicalConditionHydrationEvent e;
			e.zPlayerActor = this;
			NotifyObservers(&e);
		}
	}
}

bool PlayerActor::UpdateBreathSoundTimer( float dt )
{
	this->zBreathSoundTimer -= dt;
	if(this->zBreathSoundTimer > 0.0f)
		return false;
	else
	{
		this->zBreathSoundTimer = 1.5f;
		return true;
	}
}
