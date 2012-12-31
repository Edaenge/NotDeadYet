#include "GameFiles/ServerSide/PlayerActor.h"

PlayerActor::PlayerActor( const int ID ) : BioActor()
{
	InitValues();
	this->SetID(ID);
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : BioActor(startPos)
{
	InitValues();
	this->SetID(ID);
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : BioActor(startPos, startRot)
{
	InitValues();
	this->SetID(ID);
}

void PlayerActor::InitValues()
{
	this->zObjManager = NULL;
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
	this->zHunger = 100.0f;
	this->zHydration = 100.0f;
	this->zInventory = new Inventory();
}

PlayerActor::~PlayerActor()
{
	SAFE_DELETE(zInventory);
}

void PlayerActor::Update(float deltaTime)
{
	float dt = deltaTime + this->zLatency;

	if(this->zKeyStates.GetKeyState(KEY_SPRINT))
	{
		if(Sprint(dt))
			this->zState = STATE_RUNNING;
		else
			this->zState = STATE_WALKING;
	}

	else if(this->zKeyStates.GetKeyState(KEY_DUCK))
		this->zState = STATE_CROUCHING;

	else
		this->zState = STATE_WALKING;
	
	this->zFrameTime += dt;
	switch (this->zState)
	{
	case STATE_WALKING:
		this->zVelocity = V_WALK_SPEED;
		break;
	case STATE_RUNNING:
		this->zVelocity = V_RUN_SPEED;
		break;
	case STATE_CROUCHING:
		this->zVelocity = V_CROUCH_SPEED;
		break;
	case STATE_IDLE:
		break;
	default:
		break;
	}

	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		this->zPos = this->zPos + this->zDir * dt * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		this->zPos = this->zPos + this->zDir * -1 * dt * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * dt * this->zVelocity);
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * -1 * dt * this->zVelocity);
	}

	if(this->zState != STATE_RUNNING && (this->zStamina != this->zStaminaMax))
	{
		this->zStamina += dt * this->zStaminaCof;

		if(this->zStamina > this->zStaminaMax)
			this->zStamina = this->zStaminaMax;
	}
}

bool PlayerActor::PickUpObject( DynamicObjectActor* object )
{
	//Not yet implemented
	return false;
}

bool PlayerActor::PickUpObject( StaticObjectActor* object )
{
	FoodObject* fo		= NULL;
	WeaponObject* wo	= NULL;
	Item* item			= NULL; 

	fo = dynamic_cast<FoodObject*>(object);
	if(fo)
	{
		item = new Food(fo->GetID(), fo->GetWeight(), fo->GetActorObjectName(), fo->GetType(), fo->GetDescription() ,fo->GetHunger());
		
		if(!this->zInventory->AddItem(item))
		{
			SAFE_DELETE(item);
			return false;
		}

		return true;
	}
	
	wo = dynamic_cast<WeaponObject*>(object);
	if(wo)
	{
		
		switch (wo->GetType())
		{
		case ITEM_TYPE_WEAPON_MELEE_AXE:
			item = new MeleeWeapon(wo->GetID(), wo->GetWeight(), wo->GetActorObjectName(), wo->GetType(),
									wo->GetDescription(), wo->GetDamage(), wo->GetRange());
			break;
		case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
			item = new MeleeWeapon(wo->GetID(), wo->GetWeight(), wo->GetActorObjectName(), wo->GetType(),
				wo->GetDescription(), wo->GetDamage(), wo->GetRange());
			break;
		case ITEM_TYPE_WEAPON_RANGED_BOW:
			item = new RangedWeapon(wo->GetID(), wo->GetWeight(), wo->GetActorObjectName(), wo->GetType(),
				wo->GetDescription(), wo->GetDamage(), wo->GetRange());
			break;
		case ITEM_TYPE_WEAPON_RANGED_ROCK:
			item = new RangedWeapon(wo->GetID(), wo->GetWeight(), wo->GetActorObjectName(), wo->GetType(),
				wo->GetDescription(), wo->GetDamage(), wo->GetRange());
			break;
		default:
			//Return
			return false;
			break;
		}

		if(!this->zInventory->AddItem(item))
		{
			SAFE_DELETE(item);
			return false;
		}
		return true;
	}

	return false;
}

Item* PlayerActor::DropObject( const int ID )
{
	Item* item = this->zInventory->SearchAndGetItem(ID);
	unsigned int item_type;

	if(!item)
		return NULL;

	this->zInventory->RemoveItem(item);

	return item;
}
