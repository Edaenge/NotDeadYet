#include "PlayerActor.h"

PlayerActor::PlayerActor(const long ID) : BioActor()
{
	InitValues();
	this->SetID(ID);
}

PlayerActor::PlayerActor(const long ID, const Vector3& startPos, PhysicsObject* pObj) : BioActor(startPos, pObj)
{
	InitValues();
	this->SetID(ID);
}

PlayerActor::PlayerActor(const long ID, const Vector3& startPos, const Vector4& startRot, PhysicsObject* pObj) : BioActor(startPos, pObj, startRot)
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
	this->zInitialDirection = Vector3(0,0,-1);
	this->zInventory = new Inventory();
	this->zEquipment = new Equipment();
	this->zActorType = ACTOR_TYPE_PLAYER;

	this->zHealthChanged = false;
	this->zStaminaChanged = false;
	this->zHunger = false;
	this->zHydration = false;
}

PlayerActor::~PlayerActor()
{
	SAFE_DELETE(this->zInventory);
	SAFE_DELETE(this->zEquipment);
}

void PlayerActor::Update(float deltaTime)
{
	float dt = deltaTime + this->zLatency;
	this->zPreviousPos = GetPosition();
	Vector3 modified = GetPosition();

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
	Vector3 moveMenT = Vector3(0,0,0);
	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		moveMenT += this->zDirection;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		moveMenT -= this->zDirection;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
		moveMenT += right;
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
		moveMenT -= right;
	}
	moveMenT.Normalize();
	moveMenT *= dt * this->zVelocity;
	modified += moveMenT;

	if(this->zState != STATE_RUNNING && (this->zStamina < this->zStaminaMax))
	{
		this->zStamina += dt * this->zStaminaCof;

		if(this->zStamina > this->zStaminaMax)
			this->zStamina = this->zStaminaMax;

		this->zStaminaChanged = true;
	}

	bool validMove = false;
	SetPosition(modified);

	PlayerUpdatedEvent temp = PlayerUpdatedEvent(this, validMove, this->zPreviousPos);
	NotifyObservers( &temp);
	if(!temp.validMove)
		SetPosition(temp.prevPos);

}

bool PlayerActor::PickUpObject(DynamicObjectActor* object)
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
		item = new Projectile(spo->GetID(), spo->GetType(), spo->GetSpeed(), spo->GetDamage());

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

bool PlayerActor::DropObject(const long ID)
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

	this->zHydrationChanged = true;
}

void PlayerActor::EatFood(float hunger)
{
	this->zHunger += hunger;
	if (this->zHunger >= this->zHungerMax)
		this->zHunger = this->zHungerMax;

	this->zHungerChanged = true;
}

std::string PlayerActor::ToMessageString( NetworkMessageConverter* NMC )
{
	string msg = "";
	msg = BioActor::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_STATE, this->zState);
	msg += NMC->Convert(MESSAGE_TYPE_FRAME_TIME, this->zFrameTime);

	if(zHungerChanged)
	{
		msg += NMC->Convert(MESSAGE_TYPE_HUNGER, this->zHunger);
		this->zHungerChanged = false;
	}
	if(zHydrationChanged)
	{
		msg += NMC->Convert(MESSAGE_TYPE_HYDRATION, this->zHydration);
		this->zHydrationChanged = false;
	}

	return msg;
}
