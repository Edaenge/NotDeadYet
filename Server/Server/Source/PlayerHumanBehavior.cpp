#include <World/World.h>
#include "PlayerHumanBehavior.h"
#include "Player.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "PlayerConfiguration.h"

const float MAX_VELOCITY_RUN = 2.95f;
const float MAX_VELOCITY_SPRINT = 5.90f;
const float MAX_VELOCITY_DUCK = 0.1f;
const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
const float ELASTICITY = 0.5f;
const float ACCELERATION = 10000.0f;
const float GROUNDFRICTION = 0.4f;
const float AIRFRICTION = 0.95f;
const float AIRDENSITY = 1.225f;
const float DRAGCOOEFICIENT = 0.6f;
const float HUMANSURFACEAREA = 1.0f;
const float HUMANWEIGHT = 80.0f;

PlayerHumanBehavior::PlayerHumanBehavior( Actor* actor, World* world, Player* player ) : 
	PlayerBehavior(actor, world, player)
{
	this->zIntervalCounter = 0.0f;
}

PlayerHumanBehavior::~PlayerHumanBehavior()
{

}

bool PlayerHumanBehavior::Update( float dt )
{
	this->zIntervalCounter += dt;

	if ( !zPlayer )
		return true;

	if ( PlayerBehavior::Update(dt) )
		return true;

	KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 curPosition = this->zActor->GetPosition();

	// Movement Keys
	float forwardMovement = (float)(keyStates.GetKeyState(KEY_FORWARD) - keyStates.GetKeyState(KEY_BACKWARD));
	float sideMovement = (float)(keyStates.GetKeyState(KEY_RIGHT) - keyStates.GetKeyState(KEY_LEFT));

	if ( forwardMovement != 0.0f || sideMovement != 0.0f )
	{
		Vector3 moveDir = Vector3(0.0f, 0.0f, 0.0f);

		// Get Directions
		Vector3 playerForwardVector = this->zActor->GetDir();
		playerForwardVector.y = 0; // Need a vector that is horizontal to X and Z
		playerForwardVector.Normalize();
		Vector3 playerRightVector = Vector3(0.0f, 1.0f, 0.0f).GetCrossProduct(playerForwardVector);
		playerRightVector.Normalize();

		// Calc the movement vector
		moveDir += playerForwardVector * forwardMovement;
		moveDir += playerRightVector * sideMovement;
		moveDir.Normalize();
		moveDir *= ACCELERATION;

		// Movement Velocity
		zVelocity += moveDir * dt;
	}

	// Check Ground
	bool isOnGround = false;
	float heightAboveGround = 0.0f;

	// Apply Velocity
	Vector3 newPosition = Vector3(0, 0, 0);

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		heightAboveGround = curPosition.y - groundHeight;
		if( heightAboveGround <= 0.0f ) 
		{
			isOnGround = true;
			curPosition.y = groundHeight;
			zVelocity.y = 0.0f;
		}
	}
	catch(...)
	{

	}

	if ( isOnGround )
	{
		zVelocity -= ( zVelocity * 4.0f ) * dt;
	}

	newPosition = curPosition + zVelocity * dt;

	// Air Resistance
	Vector3 airResistance(0.0f, 0.0f, 0.0f);
	if ( zVelocity.x != 0.0f ) airResistance.x = powf(zVelocity.x, 2.0f);
	if ( zVelocity.y != 0.0f ) airResistance.y = powf(zVelocity.y, 2.0f);
	if ( zVelocity.z != 0.0f ) airResistance.z = powf(zVelocity.z, 2.0f);

	if ( zVelocity.x >= 0.0f ) airResistance.x *= -1.0;
	if ( zVelocity.y >= 0.0f ) airResistance.y *= -1.0;
	if ( zVelocity.z >= 0.0f ) airResistance.z *= -1.0;
	airResistance *= 0.5f * AIRDENSITY * DRAGCOOEFICIENT * HUMANSURFACEAREA;
	zVelocity -= airResistance / HUMANWEIGHT * dt;

	BioActor* bActor = dynamic_cast<BioActor*>(zActor);
	// Check Max Speeds
	if(keyStates.GetKeyState(KEY_DUCK))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_DUCK)
		{
			bActor->SetState(STATE_CROUCHING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_DUCK;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_DUCK)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}
	}
	else if(!keyStates.GetKeyState(KEY_SPRINT))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_RUN)
		{
			bActor->SetState(STATE_WALKING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_RUN;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_RUN/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}
	}
	else if(keyStates.GetKeyState(KEY_SPRINT) && bActor->GetStamina() > 0)
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_SPRINT)
		{
			bActor->SetState(STATE_RUNNING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_SPRINT;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_SPRINT/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}
	}

	Vector3 groundNormal;

	// Gravity
	if(!isOnGround)
		this->zVelDown += -9.82f * dt;
	else
	{
		groundNormal = this->zWorld->CalcNormalAt(newPosition.GetXZ());
		this->zVelDown = 0.0f;
	}
	newPosition += Vector3(0.0f, this->zVelDown, 0.0f) + (groundNormal * dt);
	//Look so player isn't outside of World.
	if ( zWorld->IsInside(newPosition.GetXZ()) )
	{
		zActor->SetPosition(newPosition);
	}
	else
	{
		zVelocity = Vector3(0.0f, 0.0f, 0.0f);

		// Move To Center of World
		Vector3 center;
		center.x = zWorld->GetWorldCenter().x;
		center.z = zWorld->GetWorldCenter().y;
		center.y = zWorld->CalcHeightAtWorldPos(Vector2(center.x, center.z));
		zActor->SetPosition(center);
	}

	PhysicalConditionCalculator(dt);

	return false;
}

void PlayerHumanBehavior::PhysicalConditionCalculator(float dt)
{
	//BioActor* bActor = dynamic_cast<BioActor*>(this->zActor);
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActor);
	
	if(this->zIntervalCounter >= 1.0f)
	{
		/*std::stringstream tester; 
		tester<<"Fullness: "<<pActor->GetFullness()<<"   "<<"Hydration: "<<pActor->GetHydration()<<"    "<<"Stamina: "<<pActor->GetStamina()<<"    "<<"Health: "<<pActor->GetHealth()<<std::endl;
		OutputDebugString(tester.str().c_str());*/
	
		float regeneratedHealth = 0;
		
		//Regaining stamina (if not running and not bleeding)
		if(pActor->GetState() != STATE_RUNNING && pActor->GetStamina() < pActor->GetStaminaMax() && !pActor->IsBleeding())
		{
			float stamina = pActor->GetStamina();
			pActor->SetStamina(stamina += 1.0f);
		
			if(pActor->GetStamina() > pActor->GetStaminaMax())
				pActor->SetStamina(pActor->GetStaminaMax());

			pActor->StaminaHasChanged();
		}

		if(pActor->GetState() == STATE_RUNNING)
		{
			float fullness; 
			float hydration;
			fullness = pActor->GetFullness(); 
			hydration = pActor->GetHydration();

			fullness -= GetPlayerConfiguration().zHungerSprintingCof;
			hydration -= GetPlayerConfiguration().zHydrationSprintingCof;
			pActor->SetFullness(fullness);
			pActor->SetHydration(hydration);
		}
		else
		{
			float fullness;
			float hydration;
			fullness = pActor->GetFullness(); 
			hydration = pActor->GetHydration();

			fullness -= GetPlayerConfiguration().zHungerCof;
			hydration -= GetPlayerConfiguration().zHydrationCof;
			pActor->SetFullness(fullness);
			pActor->SetHydration(hydration);
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

			stamina -= GetPlayerConfiguration().zStaminaDecreaseWithBleedingCof;
			pActor->SetStamina(stamina);
			fullness -= GetPlayerConfiguration().zHungerDecreaseWithBleedingCof;
			pActor->SetFullness(fullness);
			hydration -= GetPlayerConfiguration().zHydrationDecreaseWithBleedingCof;
			pActor->SetHydration(hydration);
		
		}
	
		if(pActor->GetFullness() / GetPlayerConfiguration().zFullnessMax > GetPlayerConfiguration().zUpperHunger && !pActor->IsBleeding()) //The hunger is at a good level
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationHungerAddition;
		}
		else if(pActor->GetFullness() / GetPlayerConfiguration().zFullnessMax < GetPlayerConfiguration().zLowerHunger) //The hunger is at a bad level
		{
			float stamina = pActor->GetStamina();
			stamina -= GetPlayerConfiguration().zStaminaDecreaseCofWithHunger;
			pActor->SetStamina(stamina);
		}

		if(pActor->GetHydration() / GetPlayerConfiguration().zHydrationMax > GetPlayerConfiguration().zUpperHydration && !pActor->IsBleeding()) //The thirst is at a good level
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationHydrationAddition;
		}
		else if(pActor->GetHydration() / GetPlayerConfiguration().zHydrationMax < GetPlayerConfiguration().zLowerHydration) //The thirst is at a bad level.
		{
			float stamina = pActor->GetStamina();
			stamina -= GetPlayerConfiguration().zStaminaDecreaseCofWithHydration;
			pActor->SetStamina(stamina);
		}

		if(pActor->GetStamina() / pActor->GetStaminaMax() > GetPlayerConfiguration().zUpperStamina && !pActor->IsBleeding())
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationStaminaAddition;
		}
		else if(pActor->GetStamina() / pActor->GetStaminaMax() < GetPlayerConfiguration().zLowerStamina)
		{
			float fullness = pActor->GetFullness();
			float hydration = pActor->GetHydration();
			fullness -= GetPlayerConfiguration().zHungerForStaminaCof;
			pActor->SetFullness(fullness);
			hydration -= GetPlayerConfiguration().zHydrationForStaminaCof;
			pActor->SetHydration(hydration);
		}

		Damage hurting;
		if(pActor->GetFullness() < 0)
		{
			pActor->SetFullness(0.0f);
			hurting.blunt = GetPlayerConfiguration().zDamageAtStarvationCof;
			pActor->TakeDamage(hurting, pActor);
		}
		if(pActor->GetHydration() < 0)
		{
			pActor->SetHydration(0.0f);
			hurting.blunt = GetPlayerConfiguration().zDamageAtThirstCof;
			pActor->TakeDamage(hurting, pActor);
		}

		float health = pActor->GetHealth();
		health += regeneratedHealth / GetPlayerConfiguration().zRegenerationScale;
		pActor->SetHealth(health);  
		if(pActor->GetHealth() > pActor->GetHealthMax())
		{
			pActor->SetHealth(pActor->GetHealthMax()); 
		}
		this->zIntervalCounter = 0.0f;
	}
}