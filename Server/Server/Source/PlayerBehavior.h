#pragma once

#include "Behavior.h"
#include <Vector.h>
#include "PlayerConfigReader.h"

#define TODEGREES (180 / 3.1415f)

class Player;

const float SIGHTRADIUS = 200.0f;

class PlayerBehavior : public Behavior
{
public:
	PlayerBehavior(Actor* actor, World* world, Player* player);
	virtual	~PlayerBehavior();

	bool ProcessClientData(const Vector3& direction, const Vector4& rotation);
	inline Player* GetPlayer() const {return this->zPlayer;}

	bool Update(float dt);

protected:
		virtual Actor* HandleCollision();
		virtual bool HandleWaterCollision();

protected:
	Player* zPlayer;
	Vector3 zVelocity;
	float	zVelDown;
	float zLenght;
};