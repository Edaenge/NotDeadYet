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

	inline std::set<Actor*> GetNearActors() const {return this->zNearActors;}
	std::set<Actor*> GetNearBioActors() const {return this->zNearBioActors;}
	std::set<Actor*> GetNearWorldActors() const {return this->zNearWorldActors;}

	float GetNearByRadius() const {return this->zNearByRadius;}


	bool ProcessClientData(Vector3 direction, Vector4 rotation);
	inline Player* GetPlayer() const {return this->zPlayer;}

	/*! Check collisions using distance.*/
	virtual Actor* CheckCollision( const Vector3& pos, const float& radius, const std::set<Actor*>& actors);

	bool RefreshNearCollideableActors(const std::set<Actor*>& actors);

	bool Update(float dt);

protected:
	virtual Actor* CheckBioActorCollision();
	virtual Actor* CheckWorldActorCollision();

protected:
	Player* zPlayer;
	Vector3 zVelocity;
	float	zVelDown;
	PlayerConfigReader* zPlayerConfigReader;	
	float zNearByRadius;
	unsigned int zNearActorsIndex;

	std::set<Actor*> zNearActors;
	std::set<Actor*> zNearWorldActors;
	std::set<Actor*> zNearBioActors;

};