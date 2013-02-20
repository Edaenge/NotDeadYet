#pragma once
#include <World/World.h>
#include "Behavior.h"
#include "Actor.h"
#include "DeerActor.h"
#include "AI.h"

enum MentalState
{
	IDLE, 
	CALM, 
	SUSPICIOUS,
	AGGRESSIVE, 
	AFRAID
};

enum Type
{
	HUMAN,
	DEER,
	WOLF,
	BEAR
};

struct Target //Supposed to represent an entity near the animal, like a player, or possibly other animal (removed until deemed necessary)
{
	Vector3 position;
	//float	threatLevel;
	float	health;
	float	movementNoise; //The velocity of movement, currently used here to determine how much an animal should hear.
	bool	valid;
	Type	kind;
};

class AIDeerBehavior : public Behavior
{
private:
	bool InitPathfinder();
	Vector3		ExaminePathfindingArea();

public:
	AIDeerBehavior(Actor* actor, World* world);
	virtual ~AIDeerBehavior() {}

	bool InitValues();
	bool SetValuesFromFile();

	MentalState	GetMentalState();
	void		SetMentalState(MentalState newMentalState);

	bool		GetIfNeedPath();
	void		SetIfNeedPath(bool needPath);
	float		GetFearLevel();
	void		SetFearLevel(float fear);
	float		GetFearMax();
	void		SetFearMax(float max);

	float		GetLastDistanceCheck();
	void		SetLastDistanceCheck(float distance);
	
	void		SetTargetInfo(int number, Vector3 pos, float velocity, float health, Type kind = HUMAN);
	int			GetCurrentTargets();
	void		SetCurrentTargets(int number);

	std::vector<Vector2>& GetPath();

	float		GetPreviousHealth();
	void		SetPreviousHealth(float oldHealth);

	bool Update(float dt);

private:
		AI		zPathfinder;

		std::vector<Vector2>	zCurrentPath;
		std::vector<Target>		zTargets; //Humans and other animals
		Target					zMainTarget;

		float	zCalmActionInterval;

		float	zFearIntervalCounter; 
		float	zIntervalCounter;

		//Variables read in from file
		float		zFearInterval;
		int			zMinimumDistance; //The minimum distance for an animal to be made aware of other animals and players.
		float		zFieldOfView; //The viewfield of the animal,  -1 is 360 degrees, 1 is 0 degrees.
		float		zThreatMovementSpeedThreshold; //The speed that an animal must exceed to be more noticable.
		float		zConfidenceKoef; //Made to more easily vary how afraid the animal will get each second. < 1 is less afraid, > is more afraid.
		float		zTooCloseInDistance; //The distance where a threat comes too close. Personal space range, basically.
		float		zExtraFearWithNumberOfPlayers; //The fear is going to be this * number of players. So each second the fear can increase by a variying amount.
		float		zExtraFearWithCloseProximity; //The fear to increase when a threat is under the zTooCloseInDistance variable
		float		zExtraFearAtSight; //The extra fear the animal will experience when the threat is in its view.
		float		zExtraFearAtThreatMovementSpeed; //If the threat goes over the zThreatMovementSpeedThreshold, then this will be added to the fear each second.
		float		zFearDecrease; //The amount of fear will be decreased each second when the animal is in the clear.
		float		zFearAtDamageDone; //The direct amount of fear that will immediately be added if the animal is wounded.
		float		zSupspiciousToAggressiveThreshold; //The line between an animal being suspicious and aggressive.
		float		zAggressiveToAfraidThreshold; //The line between an animal being aggressive and afraid.
		int			zDistanceToWalkWhenCalm; //The range the animal walks when relaxed. It is actually roughly half of this, with varying results
		int			zDistanceToWalkWhenSuspicious;// The range the animal walks when suspicious. It is actually roughly half of this, with varying results
		int			zNewTargetCloseByAFactorOf; //If the animal is hunting someone, if a new possible target is closer by a factor of this variable, then chase that target instead.
		int			zFleeDistance; //The distance the animal will try to flee when it is afraid.
		int			zLargeSoundEventFearIncrease; //The amount of fear that will directly be added to the animal if, say, a flare goes off or there is a large sound.

		//These are basically to ensure random pauses in the movement behaviour, as animals sometimes might do.
		int		zCalmRandomInterval; //The value that will be used with rand() % value 
		int		zCalmRandomAddition; //the value that will be added, ensuring a minimum of this value.
		int		zSupsiciousRandomInterval; //The value that will be used with rand() % value
		int		zSupsiciousAddition;//the value that will be added, ensuring a minimum of this value.
		//

		float		zCurrentDistanceFled;
		float		zWalkingVelocity;
		float		zAttackingVelocity;
		float		zFleeingVelocity;


		float		zPreviousHealth; //Used to detect if the deer has been wounded.
		float		zFearMax;
		float		zFearLevel;
		bool		zNeedPath;
		float		zLastDistanceCheck;
		int			zCurrentNrOfTargets;

		MentalState zMentalState;
		bool zPanic;
		Vector3		zDestination;
};