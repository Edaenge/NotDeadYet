/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/Actor.h"
#include "GameFiles/AnimationStates.h"
#include <vector>

class PlayerActor : public Actor
{
public:
	PlayerActor();
	PlayerActor(const int ID);
	PlayerActor(const int ID, const Vector3& startPos);
	PlayerActor(const int ID, const Vector3& startPos, const Vector4& startRot);

	virtual ~PlayerActor();

	void Update();
	

	int GetID() const;
	float GetVelocity() const;
	int GetState() const;
	const std::string& GetPlayerModel() const;
	const Vector3& GetDirection() const;

	void SetState(int state);
	void SetVelocity(float velocity);
	void SetDirection(const Vector3& dir);
	void SetPlayerModel(const std::string& model);

private:
	int		zID;
	int		zState;
	float	zVelocity;

	std::string zPlayerModel;
	
	Vector3 zDir;
};