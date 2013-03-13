#include <World/World.h>
#include "PlayerHumanBehavior.h"
#include "Player.h"
#include "Actor.h"
#include "PlayerActor.h"

const float MAX_VELOCITY_RUN = 2.95f;
const float MAX_VELOCITY_SPRINT = 5.90f;
const float MAX_VELOCITY_DUCK = 1.0f;
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

	if(BioActor *bActor = dynamic_cast<BioActor*>(this->zActor))
	{
		if(bActor->InAction())
		{
			this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
			return false;
		}
	}
	
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

	// Check On Ground
	bool isOnGround = false;
	float heightAboveGround = 0.0f;

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		heightAboveGround = curPosition.y - groundHeight;
		if( heightAboveGround <= 0.0f) 
		{
			isOnGround = true;
			curPosition.y = groundHeight;
			zVelocity.y = 0.0f;
		}
	}
	catch(...)
	{

	}

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

	//Check if fall/slide
	Vector3 groundNormal = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 normGround = zWorld->CalcNormalAt(curPosition.GetXZ());

	//Calc the vector in the plane.
	Vector3 groundProj = normGround;
	groundProj.y = 0;
	groundProj.Normalize();

	float angle = acos(normGround.GetDotProduct(groundProj));
	angle = angle * TODEGREES;
	if(angle < 45.0f)
	{
		zVelocity = Vector3(0.0f, 0.0f, 0.0f);
		groundNormal = this->zWorld->CalcNormalAt(curPosition.GetXZ()) * 4.0f;
		this->zVelDown = 0.0f;
	}
	else
		zVelocity -= ( zVelocity * 4.0f ) * dt;

	if(!isOnGround)
		this->zVelDown += -9.82f * dt; // Gravity

	BioActor* bActor = dynamic_cast<BioActor*>(zActor);
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(zActor);

	int currentState = bActor->GetState();
	//static float animationTimer = 0.0f;
	//static const float ANIMATION_WAIT_TIME = 1.0f;

	//bool bShortAnimation = false;
	//if (currentState == STATE_EQUIP_WEAPON || currentState == STATE_UNEQUIP_WEAPON)
	//	bShortAnimation = true;

	//animationTimer += dt;
	int newState = -1;

	// Check Max Speeds
	if(keyStates.GetKeyState(KEY_DUCK))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_DUCK)
		{
			newState = STATE_CROUCHING;
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_DUCK;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_DUCK/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				newState = STATE_IDLE;
				this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}
	else if(keyStates.GetKeyState(KEY_SPRINT) && !pActor->GetExhausted())
	{
		pActor->SetHasSprinted(true);

		if(this->zVelocity.GetLength() > MAX_VELOCITY_SPRINT)
		{
			newState = STATE_RUNNING;
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_SPRINT;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_SPRINT/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				newState = STATE_IDLE;
				this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}
	else
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_RUN)
		{
			newState = STATE_WALKING;
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_RUN;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_RUN/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				newState = STATE_IDLE;
				this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}
	Vector3 dir = this->zActor->GetDir();
	if(this->zVelocity.GetDotProduct(dir) < 0)
	{
		float lSpeed = this->zVelocity.GetLength();
		this->zVelocity.Normalize();
		this->zVelocity *= (lSpeed * 0.5f);
	}
	// Apply Velocity
	Vector3 newPosition = curPosition + (zVelocity * dt) + Vector3(0.0f, this->zVelDown, 0.0f) + (groundNormal * dt);

	//Look so player isn't outside of World.
	if ( zWorld->IsInside(newPosition.GetXZ()) )
	{
		if(!(newPosition == zActor->GetPosition()))
			zActor->SetPosition(newPosition, false);
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

	if(this->zVelocity.GetLength() > 0.1)
	{
		Vector3 pActor_rewind_dir;
		Actor* collide = NULL;
	
		/* Check Collisions against Dynamic Actors */
		collide = DistanceDynamicActorCollision();

		if( collide )
		{
			pActor_rewind_dir = ( collide->GetPosition() - zActor->GetPosition() );
			pActor_rewind_dir.Normalize();
			Vector3 target_rewind_dir = pActor_rewind_dir * -1;

			if (BioActor* bioActor = dynamic_cast<BioActor*>(collide) )
			{
				if( bioActor->IsAlive() )
				{
					if( bioActor->HasMoved() )
						bioActor->SetPosition( bioActor->GetPosition() - (target_rewind_dir * 0.25f) );

					zActor->SetPosition( zActor->GetPosition() - (pActor_rewind_dir * 0.25f) );
					zVelocity = Vector3(.0f, .0f, .0f);
				}
			}
			else
			{
				zActor->SetPosition( zActor->GetPosition() - (pActor_rewind_dir * 0.25f) );
				zVelocity = Vector3(.0f, .0f, .0f);
			}

			return false;
		}

		/* Check Collisions against Static Actors */
		collide = DistanceStaticActorCollision();

		if( collide )
		{
			pActor_rewind_dir = (collide->GetPosition() - zActor->GetPosition());
			pActor_rewind_dir.Normalize();

			zActor->SetPosition( zActor->GetPosition() - (pActor_rewind_dir * 0.25f) );
			zVelocity = Vector3(.0f, .0f, .0f);
		}
		else
		{
			//Sets position and notifies
			zActor->SetPosition(newPosition);
		}
	}

	if (newState != -1)
	{
		/*if (bShortAnimation)
		{
			if (animationTimer >= ANIMATION_WAIT_TIME)
			{
				if (newState == STATE_IDLE)
				{
					if(bActor->GetInventory()->GetPrimaryEquip())
					{
						newState = STATE_IDLE_WEAPON;
					}
				}
				bActor->SetState(newState);
			
				animationTimer = 0.0f;
			}
		}
		else
		{
			if (newState == STATE_IDLE)
			{
				if(bActor->GetInventory()->GetPrimaryEquip())
				{
					newState = STATE_IDLE_WEAPON;
				}
			}
			bActor->SetState(newState);
		}*/
		bActor->SetState(newState);
	}

	this->PhysicalConditionCalculator(dt);

	return false;
}

bool PlayerHumanBehavior::PhysicalConditionCalculator(float dt)
{
	static PlayerConfigReader* playerConfig = GetPlayerConfig();

	static const float Regen_Scale = playerConfig->GetVariable(REGEN_SCALE);
	static const float Hunger_Coeff = playerConfig->GetVariable(HUNGER_COEFF);
	static const float Fullness_Max = playerConfig->GetVariable(FULLNESS_MAX);
	static const float Lower_Hunger = playerConfig->GetVariable(LOWER_HUNGER);
	static const float Lower_Stamina = playerConfig->GetVariable(LOWER_STAMINA);
	static const float Hydration_Max = playerConfig->GetVariable(HYDRATION_MAX);
	static const float Lower_Hydration = playerConfig->GetVariable(LOWER_HYDRATION);
	static const float Hydration_Coeff = playerConfig->GetVariable(HYDRATION_COEFF);
	static const float Hunger_Sprint_Coeff = playerConfig->GetVariable(HUNGER_SPRINT_COEFF);
	static const float Stamina_Sprint_Coeff = playerConfig->GetVariable(STAMINA_SPRINT_COEFF);
	static const float Thirst_Damage_Coeff = playerConfig->GetVariable(DAMAGE_AT_THIRST_COEFF);
	static const float Hunger_Stamina_Coeff = playerConfig->GetVariable(HUNGER_FOR_STAMINA_COEFF);
	static const float Hydration_Sprint_Coeff = playerConfig->GetVariable(HYDRATION_SPRINT_COEFF);
	static const float Starvation_Damage_Coeff = playerConfig->GetVariable(DAMAGE_AT_STARVATON_COEFF);
	static const float Hydration_Stamina_Coeff = playerConfig->GetVariable(HYDRATION_FOR_STAMINA_COEFF);
	static const float Stamina_Decrease_Coeff_Hunger = playerConfig->GetVariable(STAMINA_DECREASE_COEFF_HUNGER);
	static const float Stamina_Decrease_Coeff_Hydration = playerConfig->GetVariable(STAMINA_DECREASE_COEFF_HYDRATION);
	
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActor);
	
	Damage bleedingAndConditionDamage;

	if(this->zIntervalCounter >= 1.0f)
	{
		//std::stringstream tester; 
		//tester<<"Fullness: "<<pActor->GetFullness()<<"   "<<"Hydration: "<<pActor->GetHydration()<<"    "<<"Stamina: "<<pActor->GetStamina()<<"    "<<"Health: "<<pActor->GetHealth()<<std::endl;
		//OutputDebugString(tester.str().c_str());
	
		float regeneratedHealth = 100;
		
		//Regaining stamina (if not running and not bleeding)
		if(pActor->GetState() != STATE_RUNNING && pActor->GetStamina() < pActor->GetStaminaMax() /*&& pActor->GetBleeding() == 0*/)
		{
			float stamina = pActor->GetStamina();
			pActor->SetStamina(stamina += 1.0f);
		
			if(pActor->GetStamina() > pActor->GetStaminaMax())
				pActor->SetStamina(pActor->GetStaminaMax());
		}

		if(pActor->GetState() == STATE_RUNNING)
		{
			if(pActor->GetExhausted() == false)
			{
				if(pActor->GetStamina() <= 0.0)
				{
					pActor->SetExhausted(true);
				}
			}
			if(pActor->GetExhausted() == false)
			{
				float fullness = pActor->GetFullness(); 
				float hydration = pActor->GetHydration(); 

				fullness -= Hunger_Sprint_Coeff;//zHungerSprintingCof;
				hydration -= Hydration_Sprint_Coeff;//zHydrationSprintingCof;
				pActor->SetFullness(fullness);
				pActor->SetHydration(hydration);

				if(pActor->GetHasSprinted())
				{
					float stamina = pActor->GetStamina();
					stamina -= Stamina_Sprint_Coeff;//zStaminaSprintingCof;
					pActor->SetStamina(stamina);
					if(stamina < 0.0f)
					{
						pActor->SetStamina(0.0f);
					}
					pActor->SetHasSprinted(false);
				}
			}
		}

		if(pActor->GetStamina() >= 20.0f && pActor->GetExhausted() == true)
		{
			pActor->SetExhausted(false);
		}

		float fullness = pActor->GetFullness();
		float hydration = pActor->GetHydration();

		fullness -= Hunger_Coeff;//zHungerCof;
		pActor->SetFullness(fullness);
		hydration -= Hydration_Coeff;//zHydrationCof;
		pActor->SetHydration(hydration);

		if(pActor->GetBleeding() > 0)//Player is bleeding.
		{
			regeneratedHealth -= (regeneratedHealth/3) * pActor->GetBleeding();
			/*float stamina = pActor->GetStamina();
			fullness = pActor->GetFullness();
			hydration = pActor->GetHydration();

			stamina -= GetPlayerConfiguration().zStaminaDecreaseWithBleedingCof;
			pActor->SetStamina(stamina);
			fullness -= GetPlayerConfiguration().zHungerDecreaseWithBleedingCof;
			pActor->SetFullness(fullness);
			hydration -= GetPlayerConfiguration().zHydrationDecreaseWithBleedingCof;
			pActor->SetHydration(hydration);*/
		
		}
	
		/*if(pActor->GetFullness() / GetPlayerConfiguration().zFullnessMax > GetPlayerConfiguration().zUpperHunger && !pActor->IsBleeding()) //The hunger is at a good level
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationHungerAddition;
		}
		else */if(pActor->GetFullness() / Fullness_Max < Lower_Hunger)//zLowerHunger) //The hunger is at a bad level
		{
			float stamina = pActor->GetStamina();
			stamina -= Stamina_Decrease_Coeff_Hunger;//zStaminaDecreaseCofWithHunger;
			pActor->SetStamina(stamina);
		}

		/*if(pActor->GetHydration() / GetPlayerConfiguration().zHydrationMax > GetPlayerConfiguration().zUpperHydration && !pActor->IsBleeding()) //The thirst is at a good level
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationHydrationAddition;
		}
		else*/ if(pActor->GetHydration() / Hydration_Max < Lower_Hydration) //The thirst is at a bad level.
		{
			float stamina = pActor->GetStamina();
			stamina -= Stamina_Decrease_Coeff_Hydration;
			pActor->SetStamina(stamina);
		}

		/*if(pActor->GetStamina() / pActor->GetStaminaMax() > GetPlayerConfiguration().zUpperStamina && !pActor->IsBleeding())
		{
			regeneratedHealth += GetPlayerConfiguration().zRegenerationStaminaAddition;
		}
		else */if(pActor->GetStamina() / pActor->GetStaminaMax() < Lower_Stamina)
		{
			float fullness = pActor->GetFullness();
			float hydration = pActor->GetHydration();

			fullness -= Hunger_Stamina_Coeff;
			pActor->SetFullness(fullness);

			hydration -= Hydration_Stamina_Coeff;
			pActor->SetHydration(hydration);
		}

		//Damage hurting;
		if(pActor->GetFullness() < 0)
		{
			pActor->SetFullness(0.0f);
			bleedingAndConditionDamage.internalDamage += Starvation_Damage_Coeff;
			//pActor->TakeDamage(hurting, pActor);
		}
		if(pActor->GetHydration() < 0)
		{
			pActor->SetHydration(0.0f);
			bleedingAndConditionDamage.internalDamage += Thirst_Damage_Coeff;
			//pActor->TakeDamage(hurting, pActor);
		}

		float health = 0.0f;


		if(regeneratedHealth < 0.0f)
		{
			//Damage bleedingDamage;
			bleedingAndConditionDamage.internalDamage += -(regeneratedHealth / Regen_Scale);
			//pActor->TakeDamage(bleedingDamage,pActor);
		}
		else
		{
			health = regeneratedHealth / Regen_Scale;
			pActor->SetHealth(pActor->GetHealth() + health);  
		}
		
		if(pActor->GetHealth() > pActor->GetHealthMax())
		{
			pActor->SetHealth(pActor->GetHealthMax()); 
		}

		if(bleedingAndConditionDamage.GetTotal() > 0.0f)
		{
			pActor->TakeDamage(bleedingAndConditionDamage, pActor);
		}

		this->zIntervalCounter = 0.0f;
	}
	return pActor->IsAlive();
}