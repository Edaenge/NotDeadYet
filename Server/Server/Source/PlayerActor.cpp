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

	this->zHungerChanged = true;
	this->zHydrationChanged = true;

	this->zFullnessMax = 100;
	this->zHydrationMax = 100;
	this->zType = ACTOR_TYPE_PLAYER;
	
	this->zExhausted = false;
	this->zHasSprinted = false;
}
//
//void PlayerActor::InitValues()
//{
//	this->zObjManager = NULL;
//	this->zFrameTime = 0.0f;
//	this->zLatency = 0.0f;
//	this->zHunger = 100.0f;
//	this->zHungerMax = 100.0f;
//	this->zHydration = 100.0f;
//	this->zHydrationMax = 100.0f;
//	this->zInventory = new Inventory();
//	// this->zActorType = ACTOR_TYPE_PLAYER;
//
//	this->zHungerChanged = true;
//	this->zHydrationChanged = true;
//}

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

//void PlayerActor::Update(float deltaTime)
//{
//	float dt = deltaTime + this->zLatency;
//	this->zPreviousPos = GetPosition();
//	Vector3 modified = GetPosition();
//
//	if(this->zKeyStates.GetKeyState(KEY_SPRINT))
//	{
//		if(Sprint(dt))
//		{
//			this->zState = STATE_RUNNING;
//			this->zStaminaChanged = true;
//		}
//		else
//			this->zState = STATE_WALKING;
//	}
//
//	else if(this->zKeyStates.GetKeyState(KEY_DUCK))
//		this->zState = STATE_CROUCHING;
//
//	else
//		this->zState = STATE_WALKING;
//	
//	this->zFrameTime += dt;
//	switch (this->zState)
//	{
//	case STATE_WALKING:
//		this->zVelocity = V_WALK_SPEED;
//		break;
//	case STATE_RUNNING:
//		this->zVelocity = V_RUN_SPEED;
//		break;
//	case STATE_CROUCHING:
//		this->zVelocity = V_CROUCH_SPEED;
//		break;
//	case STATE_IDLE:
//		break;
//	default:
//		break;
//	}
//	Vector3 moveMenT = Vector3(0,0,0);
//	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
//	{
//		moveMenT += this->zDirection;
//	}
//	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
//	{
//		moveMenT -= this->zDirection;
//	}
//	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
//	{
//		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
//		moveMenT += right;
//	}
//	if(this->zKeyStates.GetKeyState(KEY_LEFT))
//	{
//		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
//		moveMenT -= right;
//	}
//	moveMenT.Normalize();
//	moveMenT *= dt * this->zVelocity;
//	modified += moveMenT;
//
//	if(this->zState != STATE_RUNNING && (this->zStamina < this->zStaminaMax))
//	{
//		this->zStamina += dt * this->zStaminaCof;
//
//		if(this->zStamina > this->zStaminaMax)
//			this->zStamina = this->zStaminaMax;
//
//		this->zStaminaChanged = true;
//	}
//
//	bool validMove = false;
//	SetPosition(modified);
//
	//PlayerUpdatedEvent temp = PlayerUpdatedEvent(this, validMove, this->zPreviousPos);
//	NotifyObservers( &temp);
	//if(!temp.validMove)
		//SetPosition(temp.prevPos);

//}
//
//bool PlayerActor::PickUpObject(StaticObjectActor* object)
//{
//	Item* item					= NULL; 
//
//	int itemType = object->GetActorType(); 
//	if (itemType == ACTOR_TYPE_STATIC_OBJECT_FOOD)
//	{
//		FoodObject* fo = dynamic_cast<FoodObject*>(object);
//		if(fo)
//		{
//			item = new Food(fo->GetID(), fo->GetType(), fo->GetSubType(), fo->GetHunger());
//
//			item->SetStacking(true);
//			item->SetItemWeight(fo->GetWeight());
//			item->SetIconPath(fo->GetIconPath());
//			item->SetStackSize(fo->GetStackSize());
//			item->SetItemName(fo->GetActorObjectName());
//			item->SetItemDescription(fo->GetDescription());
//
//			if(!this->zInventory->AddItem(item))
//			{
//				SAFE_DELETE(item);
//				return false;
//			}
//			return true;
//		}
//		return false;
//	}
//
//	if (itemType == ACTOR_TYPE_STATIC_OBJECT_WEAPON)
//	{
//		WeaponObject* wo = dynamic_cast<WeaponObject*>(object);
//		if(wo)
//		{
//			switch (wo->GetType())
//			{
//			case ITEM_TYPE_WEAPON_MELEE:
//				item = new MeleeWeapon(wo->GetID(), wo->GetType(), wo->GetSubType(), wo->GetDamage(), wo->GetRange());
//				item->SetStacking(false);
//				item->SetItemWeight(wo->GetWeight());
//				item->SetIconPath(wo->GetIconPath());
//				item->SetStackSize(wo->GetStackSize());
//				item->SetItemName(wo->GetActorObjectName());
//				item->SetItemDescription(wo->GetDescription());
//				break;
//			case ITEM_TYPE_WEAPON_RANGED:
//				item = new RangedWeapon(wo->GetID(), wo->GetType(), wo->GetSubType(), wo->GetDamage(), wo->GetRange());
//				item->SetStacking(false);
//				item->SetItemWeight(wo->GetWeight());
//				item->SetIconPath(wo->GetIconPath());
//				item->SetStackSize(wo->GetStackSize());
//				item->SetItemName(wo->GetActorObjectName());
//				item->SetItemDescription(wo->GetDescription());
//				break;
//			default:
//				//Return
//				return false;
//				break;
//			}
//
//			if(!this->zInventory->AddItem(item))
//			{
//				SAFE_DELETE(item);
//				return false;
//			}
//			return true;
//		}
//		return false;
//	}
//	
//	if (itemType == ACTOR_TYPE_STATIC_OBJECT_CONTAINER)
//	{
//		ContainerObject* co = dynamic_cast<ContainerObject*>(object);
//		if(co)
//		{
//			item = new Container(co->GetID(), co->GetType(), co->GetSubType(), co->GetMaxUses(), co->GetCurrentUses());
//
//			item->SetStacking(false);
//			item->SetItemWeight(co->GetWeight());
//			item->SetIconPath(co->GetIconPath());
//			item->SetStackSize(co->GetStackSize());
//			item->SetItemName(co->GetActorObjectName());
//			item->SetItemDescription(co->GetDescription());
//
//			if(!this->zInventory->AddItem(item))
//			{
//				SAFE_DELETE(item);
//				return false;
//			}
//
//			return true;
//		}
//	}
	
	//if (itemType == ACTOR_TYPE_STATIC_OBJECT_PROJECTILE)
	//{
	//	StaticProjectileObject* spo = dynamic_cast<StaticProjectileObject*>(object);
	//	if(spo)
	//	{
	//		item = new Projectile(spo->GetID(), spo->GetType(), spo->GetSubType(), spo->GetSpeed(), spo->GetDamage());

	//		item->SetStacking(true);
	//		item->SetItemWeight(spo->GetWeight());
	//		item->SetIconPath(spo->GetIconPath());
	//		item->SetStackSize(spo->GetStackSize());
	//		item->SetItemName(spo->GetActorObjectName());
	//		item->SetItemDescription(spo->GetDescription());

	//		if(!this->zInventory->AddItem(item))
	//		{
	//			SAFE_DELETE(item);
	//			return false;
	//		}
	//		return true;
	//	}
	//	return false;
	//}
/*
if (itemType == ACTOR_TYPE_STATIC_OBJECT_MATERIAL)
{
MaterialObject*	mo = dynamic_cast<MaterialObject*>(object);
if(mo)
{
item = new Material(mo->GetID(), mo->GetType(), mo->GetSubType(), mo->GetCraftingType(), mo->GetRequiredStackToCraft());

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
return false;*/
//}
//
//
//void PlayerActor::Drink(float hydration)
//{
//	this->zHydration += hydration;
//	if (this->zHydration >= this->zHydrationMax)
//		this->zHydration = this->zHydrationMax;
//
//	this->zHydrationChanged = true;
//}
//
//void PlayerActor::EatFood(float hunger)
//{
//	this->zHunger += hunger;
//	if (this->zHunger >= this->zHungerMax)
//		this->zHunger = this->zHungerMax;
//
//	this->zHungerChanged = true;
//}
//
//std::string PlayerActor::ToMessageString( NetworkMessageConverter* NMC )
//{
//	string msg = "";
//	msg = BioActor::ToMessageString(NMC);
//
//	msg += NMC->Convert(MESSAGE_TYPE_STATE, (float)this->zState);
//	//msg += NMC->Convert(MESSAGE_TYPE_FRAME_TIME, this->zFrameTime);
//
//	if(zHungerChanged)
//	{
//		msg += NMC->Convert(MESSAGE_TYPE_HUNGER, this->zHunger);
//		this->zHungerChanged = false;
//	}
//	if(zHydrationChanged)
//	{
//		msg += NMC->Convert(MESSAGE_TYPE_HYDRATION, this->zHydration);
//		this->zHydrationChanged = false;
//	}
//
//	return msg;
//}

float PlayerActor::GetFullness()
{
	return this->zFullness;
}

float PlayerActor::GetHydration()
{
	return this->zHydration;
}

void PlayerActor::SetFullness(float fullness)
{
	this->zFullness = fullness;
}

void PlayerActor::SetHydration(float hydration)
{
	this->zHydration = hydration;
}

void PlayerActor::SetExhausted( bool exhausted )
{
	this->zExhausted = exhausted;

	if(this->zExhausted)
	{
		PlayerExhausted e;
		e.zPos = this->GetPosition();
		NotifyObservers(&e);
	}
}
