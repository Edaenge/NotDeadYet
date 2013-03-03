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

	void SetNearActors(std::set<Actor*> actors);

	inline std::set<Actor*> GetNearActors() const { return this->zNearActors;}
	float GetCollisionRadius() const { return zCollisionRadius; }

	bool ProcessClientData(Vector3 direction, Vector4 rotation);
	inline Player* GetPlayer() const {return this->zPlayer;}

	virtual Actor* CheckCollision();
	bool RefreshNearCollideableActors(const std::set<Actor*>& actors);

	bool Update(float dt);

protected:
	Player* zPlayer;
	Vector3 zVelocity;
	float	zVelDown;
	PlayerConfigReader* zPlayerConfigReader;	float zCollisionRadius;
	unsigned int zNearActorsIndex;
	std::set<Actor*> zNearActors;};