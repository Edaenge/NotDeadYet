/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "AI.h"
#include "BioActor.h"


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

struct Target //Supposed to represent an entity near the animal, like a player, or other animal.
{
	Vector3* position; //Will this work? What if the player is destroyed? Will it just point to a null, or something worse?
	float	threatLevel;//Type	type;

};

/*This is a NPC animal. It inherits from BioActor.*/
/*ID is generated in Actor.h*/
class AnimalActor : public BioActor
{
public:
	AnimalActor(bool genID = true);
	AnimalActor(const Vector3& startPos, bool genID = true);
	AnimalActor(const Vector3& startPos, const Vector4& rot, bool genID = true);
	virtual ~AnimalActor();

	virtual void Update(float deltaTime);

	Behaviour	GetBehaviour();
	void		SetBehaviour(Behaviour newBehaviour);

	bool		GetIfNeedPath();
	void		SetIfNeedPath(bool needPath);
	float		GetAlertnessLevel();
	void		SetAlertnessLevel(float alertness);
	float		GetAlertnessMax();
	void		SetAlertnessMax(float max);
	float		GetFearLevel();
	void		SetFearLevel(float fear);
	float		GetFearMax();
	void		SetFearMax(float max);

	
	int			GetLastDistanceCheck();
	void		SetLastDistanceCheck(int distance);
	//void		AddEntity(Vector3* pos, Type type);

private:
	void	InitPathfinder();

protected:
	AI		zPathfinder;
	std::vector<Vector2> zCurrentPath;
	std::vector<Target> zTargets;
	Target	zMainTarget;
	float	zCalmActionInterval;
	float	zAlertnessIntervalCounter;
	float	zFearIntervalCounter;
	float	zIntervalCounter;

private: 
	Behaviour	zMentalState;
	//Type		zType;
	bool		zNeedPath;

	float		zAlertnessLevel;
	float		zAlertnessMax;
	float		zFearMax;
	float		zFearLevel;
	
	int			zLastDistanceCheck;
	

};


//Do not forget to set values at 0 in the constructor.