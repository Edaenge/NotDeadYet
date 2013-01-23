/*
Made by Ed�nge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska h�gskola.
*/

#pragma once

#include "AI.h"
#include "BioActor.h"
#include <fstream>
#include <time.h>

enum Behaviour
{
	IDLE, 
	CALM, 
	SUSPICIOUS,
	AGGRESSIVE, 
	AFRAID
};

//enum Type
//{
//	HUMAN,
//	DEER
//};

struct Target //Supposed to represent an entity near the animal, like a player, or possibly other animal (removed until deemed necessary)
{
	Vector3 position;
	//float	threatLevel;
	float	health;
	float	movementNoise; //The velocity of movement, currently used here to determine how much an animal should hear.
	bool	valid;
};

/*This is a NPC animal. It inherits from BioActor.*/
/*ID is generated in Actor.h*/
class AnimalActor : public BioActor
{
public:
	AnimalActor(bool genID = true);
	AnimalActor(const Vector3& startPos, PhysicsObject* pObj, bool genID = true);
	AnimalActor(const Vector3& startPos, PhysicsObject* pObj, const Vector4& rot,  bool genID = true);
	virtual ~AnimalActor();

	virtual void Update(float deltaTime);
	virtual void UpdateForAnimal(float deltaTime);
	virtual void UpdateForPlayer(float deltaTime);

	Behaviour	GetBehaviour();
	void		SetBehaviour(Behaviour newBehaviour);

	bool		GetIfNeedPath();
	void		SetIfNeedPath(bool needPath);
	//float		GetAlertnessLevel();
	//void		SetAlertnessLevel(float alertness);
	//float		GetAlertnessMax();
	//void		SetAlertnessMax(float max);
	float		GetFearLevel();
	void		SetFearLevel(float fear);
	float		GetFearMax();
	void		SetFearMax(float max);

	
	int			GetLastDistanceCheck();
	void		SetLastDistanceCheck(int distance);
	
	void		SetPlayerInfo(int number, Vector3 pos, float velocity, float health);
	int			GetCurrentPlayers();
	void		SetCurrentPlayers(int number);
	virtual void LargeSuddenSoundReaction();

	std::vector<Vector2>& GetPath();

	float		GetPreviousHealth();
	void		SetPreviousHealth(float oldHealth);

	bool		GetIfPlayerControlled();
	void		SetIfPlayerControlled(bool isControlled);
	//void		AddEntity(Vector3* pos, Type type);

private:
	void	InitPathfinder();

protected:
	AI		zPathfinder;

	std::vector<Vector2>	zCurrentPath;
	std::vector<Target>		zTargets;
	Target					zMainTarget;

	float	zCalmActionInterval;
	//float	zAlertnessIntervalCounter;
	float	zFearIntervalCounter;
	float	zIntervalCounter;

private: 
	Behaviour	zMentalState;
	//Type		zType;
	bool		zNeedPath;
	bool		zIsPlayerControlled;

	//float		zAlertnessLevel;
	//float		zAlertnessMax;
	float		zPreviousHealth; //Used to detect if the animal has been wounded.
	float		zFearMax;
	float		zFearLevel;
	
	int			zLastDistanceCheck;
	
	int			zCurrentNrOfPlayers;
	

};


//Do not forget to set values at 0 in the constructor.