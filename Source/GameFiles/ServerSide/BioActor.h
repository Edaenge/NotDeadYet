/*
Made by Edänge Simon 2012-12-11
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/Actor.h"
#include "GameFiles/AnimationStates.h"
#include <string>

/*This class is used to create living creatures such as humans, animals etc. */
class BioActor : public Actor
{
public:
	BioActor() : Actor()
	{
		this->zState = STATE_IDLE;
		this->zVelocity = V_WALK_SPEED;

		this->zDir = Vector3(1,0,0);
		this->zUp = Vector3(0,1,0);
		this->zActorModel = "none";
	}
	BioActor(const Vector3& startPos) : Actor(startPos)
	{
		this->zState = STATE_IDLE;
		this->zVelocity = V_WALK_SPEED;

		this->zDir = Vector3(1,0,0);
		this->zUp = Vector3(0,1,0);
		this->zActorModel = "none";
	}
	BioActor(const Vector3& startPos, const Vector4& rot) : Actor(startPos, rot)
	{
		this->zState = STATE_IDLE;
		this->zVelocity = V_WALK_SPEED;

		this->zDir = Vector3(1,0,0);
		this->zUp = Vector3(0,1,0);
		this->zActorModel = "none";
	}

	virtual ~BioActor(){};

	virtual void Update(){};

	int GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	const std::string& GetActorModel() const {return this->zActorModel;}
	const Vector3& GetDirection() const {return this->zDir;}
	const Vector3& GetUpVector() const {return this->zUp;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state){this->zState = state;}
	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	inline void SetDirection(const Vector3& dir){this->zDir = dir;}
	void SetPlayerModel(const std::string& model){this->zActorModel = model;}
	inline void SetUpVector(const Vector3& up){this->zUp = up;}

protected:
	int		zState;
	float	zVelocity;

	std::string zActorModel;

	Vector3 zDir;
	Vector3 zUp;



};