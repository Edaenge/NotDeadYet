#include <World/World.h>
#include "PlayerHumanBehavior.h"
#include "Player.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "PlayerConfiguration.h"

const int MAX_VELOCITY = 30;const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
const float ELASTICITY = 0.5f;
const float ACCELERATION = 2.0f;
const float PLAYERHEIGHT = 1.7f;

PlayerHumanBehavior::PlayerHumanBehavior( Actor* actor, World* world, Player* player ) : 
	PlayerBehavior(actor, world, player)
{

}

PlayerHumanBehavior::~PlayerHumanBehavior()
{

}

bool PlayerHumanBehavior::Update( float dt )
{
	if ( PlayerBehavior::Update(dt) )
		return true;

	KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 newPlayerPos;
	Vector3 moveDir = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 currentPlayerPos = this->zActor->GetPosition();

	//Get Directions
	Vector3 currentPlayerDir = this->zActor->GetDir();
	Vector3 currentPlayerUp = Vector3(0.0f, 1.0f, 0.0f);
	currentPlayerDir.y = 0; // Need a vector that is horizontal to X and Z
	currentPlayerDir.Normalize();
	Vector3 currentPlayerRight = currentPlayerUp.GetCrossProduct(currentPlayerDir);
	currentPlayerRight.Normalize();
	Vector3 currentGroundNormal = this->zWorld->CalcNormalAt(currentPlayerPos.GetXZ());

	// Calc the movement vector
	moveDir += currentPlayerDir * (float)(keyStates.GetKeyState(KEY_FORWARD) - //if KEY_BACKWARD then currentPlayerDir inverse 
		keyStates.GetKeyState(KEY_BACKWARD));

	moveDir += currentPlayerRight * (float)(keyStates.GetKeyState(KEY_RIGHT) - 
		keyStates.GetKeyState(KEY_LEFT));

	moveDir.Normalize();
	moveDir *= 10.0f;

	this->zVelocity += (moveDir + GRAVITY) * dt;

	if(this->zVelocity.GetLength() > MAX_VELOCITY)
	{
		this->zVelocity.Normalize();
		this->zVelocity *= MAX_VELOCITY;
	}

	float newGroundHeight = zWorld->CalcHeightAtWorldPos(currentPlayerPos.GetXZ()) + PLAYERHEIGHT;
	if( newGroundHeight > currentPlayerPos.y )
	{
		Vector3 newGroundNormal = zWorld->CalcNormalAt(currentPlayerPos.GetXZ());
		this->zVelocity = newGroundNormal * this->zVelocity.GetLength() * ELASTICITY;
	}

	currentPlayerPos += this->zVelocity * dt;

	this->zActor->SetPosition(currentPlayerPos);
	this->zAnchor->position = newPlayerPos.GetXZ();
	this->zAnchor->radius = SIGHTRADIUS;

	PhysicalConditionCalculator(dt);

	return false;
}

void PlayerHumanBehavior::PhysicalConditionCalculator(float dt)
{
	
	//BioActor* bActor = dynamic_cast<BioActor*>(this->zActor);
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActor);

	std::stringstream tester; 
	tester<<"Fullness: "<<pActor->GetFullness()<<"   "<<"Hydration: "<<pActor->GetHydration()<<"    "<<"Stamina: "<<pActor->GetStamina()<<"    "<<"Health: "<<pActor->GetHealth()<<std::endl;

	OutputDebugString(tester.str().c_str());


	float regeneratedHealth = 0;
	

	//Regaining stamina (if not running and not bleeding)
	if(pActor->GetState() != STATE_RUNNING && pActor->GetStamina() < pActor->GetStaminaMax() && !pActor->IsBleeding())
	{
		float stamina = pActor->GetStamina();
		pActor->SetStamina( stamina += dt * pActor->IsBleeding() );

		if(pActor->GetStamina() > pActor->GetStaminaMax())
			pActor->SetStamina( pActor->GetStaminaMax() );

		pActor->StaminaHasChanged();
	}

	if(pActor->GetState() == STATE_RUNNING)
	{
		float fullness, hydration;
		fullness = pActor->GetFullness(); hydration = pActor->GetHydration();
		
		pActor->SetFullness(fullness -= dt * GetPlayerConfiguration().zHungerSprintingCof);
		pActor->SetHydration(hydration -= dt * GetPlayerConfiguration().zHydrationSprintingCof);
		
	}
	else
	{
		float fullness, hydration;
		fullness = pActor->GetFullness(); hydration = pActor->GetHydration();

		
		pActor->SetFullness(fullness -= dt * GetPlayerConfiguration().zHungerCof);
		pActor->SetHydration(hydration -= dt * GetPlayerConfiguration().zHydrationCof);

	}
	
	pActor->HungerHasChanged();
	pActor->HydrationHasChanged();


	if(pActor->IsBleeding())//Player is bleeding.
	{
		
		//Affects values:
			//- Slightly decreases hunger value
			//- Slighty decreases hydration value
			//- Slightly decreases stamina value

		float stamina = pActor->GetStamina();
		float fullness = pActor->GetFullness();
		float hydration = pActor->GetHydration();

		pActor->SetStamina( stamina -=  dt * GetPlayerConfiguration().zStaminaDecreaseWithBleedingCof);
		pActor->SetFullness( fullness -= dt * GetPlayerConfiguration().zHungerDecreaseWithBleedingCof);
		pActor->SetHydration (hydration -= dt * GetPlayerConfiguration().zHydrationDecreaseWithBleedingCof);
		
	}
	
	if(pActor->GetFullness() / GetPlayerConfiguration().zFullnessMax > GetPlayerConfiguration().zUpperHunger && !pActor->IsBleeding()) //The hunger is at a good level
	{
		regeneratedHealth += GetPlayerConfiguration().zRegenerationHungerAddition;
	}
	else if(pActor->GetFullness() / GetPlayerConfiguration().zFullnessMax < GetPlayerConfiguration().zLowerHunger) //The hunger is at a bad level
	{
		float stamina = pActor->GetStamina();
		pActor->SetStamina( stamina -=  dt * GetPlayerConfiguration().zStaminaDecreaseCofWithHunger);
	}

	if(pActor->GetHydration() / GetPlayerConfiguration().zHydrationMax > GetPlayerConfiguration().zUpperHydration && !pActor->IsBleeding()) //The thirst is at a good level
	{
		regeneratedHealth += GetPlayerConfiguration().zRegenerationHydrationAddition;
	}
	else if(pActor->GetHydration() / GetPlayerConfiguration().zHydrationMax < GetPlayerConfiguration().zLowerHydration) //The thirst is at a bad level.
	{
		float stamina = pActor->GetStamina();
		pActor->SetStamina( stamina -=  dt * GetPlayerConfiguration().zStaminaDecreaseCofWithHydration);
	}

	if(pActor->GetStamina() / pActor->GetStaminaMax() > GetPlayerConfiguration().zUpperStamina && !pActor->IsBleeding())
	{
		regeneratedHealth += GetPlayerConfiguration().zRegenerationStaminaAddition;
	}
	else if(pActor->GetStamina() / pActor->GetStaminaMax() < GetPlayerConfiguration().zLowerStamina)
	{
		float fullness = pActor->GetFullness();
		float hydration = pActor->GetHydration();
		pActor->SetFullness(fullness -= dt * GetPlayerConfiguration().zHungerForStaminaCof );
		pActor->SetHydration(hydration -= dt * GetPlayerConfiguration().zHydrationForStaminaCof );
	}

	Damage hurting;
	if(pActor->GetFullness() < 0)
	{
		pActor->SetFullness(0.0f);
		hurting.blunt = dt * GetPlayerConfiguration().zDamageAtStarvationCof;
		pActor->TakeDamage(hurting,0);
	}
	if(pActor->GetHydration() < 0)
	{
		pActor->SetHydration(0.0f);
		hurting.blunt = dt * GetPlayerConfiguration().zDamageAtThirstCof;
		pActor->TakeDamage(hurting,0);
	}


	float health = pActor->GetHealth();
	pActor->SetHealth( health += regeneratedHealth * dt / GetPlayerConfiguration().zRegenerationScale);  
	if(pActor->GetHealth() > pActor->GetHealthMax())
	{
		pActor->SetHealth( pActor->GetHealthMax() ); 
	}
}


