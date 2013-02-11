#pragma once

#include "Actor.h"
#include "Damage.h"

class ProjectileActor : public Actor
{
	
public:
	ProjectileActor(Actor* owner, PhysicsObject* pObj);
	virtual ~ProjectileActor();

	Actor* GetOwner() {return this->zOwner;}
	Damage& GetDamage() {return this->zDamage;}

	void SetDamage(const Damage& dmg) {this->zDamage = dmg;}

protected:
	Actor* zOwner;
	Damage zDamage;

private:

};