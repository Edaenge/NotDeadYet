#include "PlayerActor.h"

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
	this->zHungerMax = 100.0f;
	this->zHydration = 100.0f;
	this->zHydrationMax = 100.0f;
	this->zInventory = new Inventory();
	this->zEquipment = new Equipment();

	this->zHealthChanged = false;
	this->zStaminaChanged = false;
	this->zHunger = false;
	this->zHydration = false;
}

PlayerActor::~PlayerActor()
{
	SAFE_DELETE(zInventory);
}

void PlayerActor::Update(float deltaTime)
{
	float dt = deltaTime + this->zLatency;
	this->zPreviousPos = this->zPos;
	bool changed = false;

	if(this->zKeyStates.GetKeyState(KEY_SPRINT))
	{
		if(Sprint(dt))
		{
			this->zState = STATE_RUNNING;
			this->zStaminaChanged = true;
		}
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
		changed = true;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		this->zPos = this->zPos + this->zDir * -1 * dt * this->zVelocity;
		changed = true;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * dt * this->zVelocity);
		changed = true;
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * -1 * dt * this->zVelocity);
		changed = true;
	}

	if(this->zState != STATE_RUNNING && (this->zStamina < this->zStaminaMax))
	{
		this->zStamina += dt * this->zStaminaCof;

		if(this->zStamina > this->zStaminaMax)
			this->zStamina = this->zStaminaMax;

		this->zStaminaChanged = true;
	}
	bool validMove = false;
	if(changed)
	{
		PlayerUpdatedEvent temp = PlayerUpdatedEvent(this, validMove);
		NotifyObservers( &temp);
		if(temp.validMove)
			this->zPhysicObj->SetPosition(this->zPos);
		else
		{
			this->zPos = zPreviousPos;
		}
	}
}

bool PlayerActor::PickUpObject( DynamicObjectActor* object)
{
	//Not yet implemented
	return false;
}

bool PlayerActor::PickUpObject(StaticObjectActor* object)
{
	FoodObject* fo				= NULL;
	WeaponObject* wo			= NULL;
	ContainerObject* co			= NULL;
	StaticProjectileObject* spo = NULL;
	MaterialObject*	mo			= NULL;
	Item* item					= NULL; 

	fo = dynamic_cast<FoodObject*>(object);
	if(fo)
	{
		item = new Food(fo->GetID(), fo->GetType(), fo->GetHunger());

		item->SetStacking(true);
		item->SetItemWeight(fo->GetWeight());
		item->SetIconPath(fo->GetIconPath());
		item->SetStackSize(fo->GetStackSize());
		item->SetItemName(fo->GetActorObjectName());
		item->SetItemDescription(fo->GetDescription());
		
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
			item = new MeleeWeapon(wo->GetID(), wo->GetType(), wo->GetDamage(), wo->GetRange());

			item->SetStacking(false);
			item->SetItemWeight(wo->GetWeight());
			item->SetIconPath(wo->GetIconPath());
			item->SetStackSize(wo->GetStackSize());
			item->SetItemName(wo->GetActorObjectName());
			item->SetItemDescription(wo->GetDescription());
			break;
		case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
			item = new MeleeWeapon(wo->GetID(), wo->GetType(), wo->GetDamage(), wo->GetRange());

			item->SetStacking(false);
			item->SetItemWeight(wo->GetWeight());
			item->SetIconPath(wo->GetIconPath());
			item->SetStackSize(wo->GetStackSize());
			item->SetItemName(wo->GetActorObjectName());
			item->SetItemDescription(wo->GetDescription());
			break;
		case ITEM_TYPE_WEAPON_RANGED_BOW:
			item = new RangedWeapon(wo->GetID(), wo->GetType(), wo->GetDamage(), wo->GetRange());

			item->SetStacking(false);
			item->SetItemWeight(wo->GetWeight());
			item->SetIconPath(wo->GetIconPath());
			item->SetStackSize(wo->GetStackSize());
			item->SetItemName(wo->GetActorObjectName());
			item->SetItemDescription(wo->GetDescription());
			break;
		case ITEM_TYPE_WEAPON_RANGED_ROCK:
			item = new RangedWeapon(wo->GetID(), wo->GetType(), wo->GetDamage(), wo->GetRange());

			item->SetStacking(true);
			item->SetItemWeight(wo->GetWeight());
			item->SetIconPath(wo->GetIconPath());
			item->SetStackSize(wo->GetStackSize());
			item->SetItemName(wo->GetActorObjectName());
			item->SetItemDescription(wo->GetDescription());
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

	co = dynamic_cast<ContainerObject*>(object);
	if(co)
	{
		item = new Container(co->GetID(), co->GetType(), co->GetMaxUses(), co->GetCurrentUses());
		
		item->SetStacking(false);
		item->SetItemWeight(co->GetWeight());
		item->SetIconPath(co->GetIconPath());
		item->SetStackSize(co->GetStackSize());
		item->SetItemName(co->GetActorObjectName());
		item->SetItemDescription(co->GetDescription());

		if(!this->zInventory->AddItem(item))
		{
			SAFE_DELETE(item);
			return false;
		}

		return true;
	}

	spo = dynamic_cast<StaticProjectileObject*>(object);
	if(spo)
	{
		item = new Projectile(spo->GetID(), spo->GetType(), spo->GetVelocity(), spo->GetDamage());

		item->SetStacking(true);
		item->SetItemWeight(spo->GetWeight());
		item->SetIconPath(spo->GetIconPath());
		item->SetStackSize(spo->GetStackSize());
		item->SetItemName(spo->GetActorObjectName());
		item->SetItemDescription(spo->GetDescription());

		if(!this->zInventory->AddItem(item))
		{
			SAFE_DELETE(item);
			return false;
		}

		return true;
	}

	mo = dynamic_cast<MaterialObject*>(object);
	if(mo)
	{
		item = new Material(mo->GetID(), mo->GetType(), mo->GetCraftingType(), mo->GetRequiredStackToCraft());

		item->SetStacking(true);
		item->SetItemWeight(mo->GetWeight());
		item->SetIconPath(mo->GetIconPath());
		item->SetStackSize(mo->GetStackSize());
		item->SetItemName(mo->GetActorObjectName());
		item->SetItemDescription(mo->GetDescription());

		if(!this->zInventory->AddItem(item))
		{
			SAFE_DELETE(item);
			return false;
		}

		return true;
	}

	return false;
}

bool PlayerActor::DropObject( const int ID )
{
	Item* item = this->zInventory->SearchAndGetItem(ID);

	if(!item)
	{
		MaloW::Debug("Failed Item=NULL ID: " + MaloW::convertNrToString((float)ID));
		return false;
	}
	this->zInventory->RemoveItem(item);
	MaloW::Debug("Removed successes: " + MaloW::convertNrToString((float)ID));

	return true;
}

void PlayerActor::Drink(float hydration)
{
	this->zHydration += hydration;
	if (this->zHydration >= this->zHydrationMax)
		this->zHydration = this->zHydrationMax;

}

void PlayerActor::EatFood(float hunger)
{
	this->zHunger += hunger;
	if (this->zHunger >= this->zHungerMax)
		this->zHunger = this->zHungerMax;
}

void PlayerActor::AddChangedHData( string& mess, NetworkMessageConverter* nmc )
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
	if(zHungerChanged)
	{
		mess += nmc->Convert(MESSAGE_TYPE_HUNGER, this->zHunger);
		this->zHungerChanged = false;
	}
	if(zHydrationChanged)
	{
		mess += nmc->Convert(MESSAGE_TYPE_HYDRATION, this->zHydrationChanged);
		this->zHydrationChanged = false;
	}
}

void PlayerActor::RewindPosition()
{
	this->zPos = this->zPreviousPos;
}
