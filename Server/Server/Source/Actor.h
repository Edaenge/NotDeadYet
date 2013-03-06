/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include <string>
#include <Vector.h>
#include <Safe.h>
#include <Observer.h>
#include "PhysicsObject.h"
#include "ActorEvent.h"
static const enum ACTOR_TYPE
{
	ACTOR_TYPE_NONE,
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_GHOST,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_WORLD,
	ACTOR_TYPE_PROJECTILE,
	ACTOR_TYPE_ITEM
};

class Actor;
/* 
   Every time an actor is created, an ID will be generated in this class. This id should be used for this actor.
   An exception is Players. They have the same ID as the ClientChannel.
*/
class Actor : public Observed
{

private:
	Vector3 zPos;
	Vector3 zPreviousPos;
	Vector4 zRot;
	Vector3 zDir;
	Vector3 zScale;

	unsigned int zID;
	PhysicsObject* zPhysicsObject;

protected:
		unsigned int zType;
		float zEnergy;
		float	zCollisionRadius;
		Vector2 zCollisionPoints[4];		std::string zModel;
protected:
	virtual void CalculateCollisionPoints();

public:
	Actor();
	virtual ~Actor();
	
	inline unsigned int GetID() const { return this->zID; }

	// Set Transformation Functions
	void SetPosition(const Vector3& pos, const bool notify = true);
	void SetRotation(const Vector4& rot, const bool notify = true);
	void SetRotation(const Vector3& around, const float angle, const bool notify = true);
	void SetPhysicsObject( PhysicsObject* object );
	void SetScale(const Vector3& scale, const bool notify = true);
	void SetDir(const Vector3& dir, const bool notify = true);
	void SetEnergy(float energy, const bool notify = true);
	virtual void SetModel(const std::string& model);	void SetCollisionRadius(const float radius) {this->zCollisionRadius = radius;}	bool CanCollide() const;

	// Get Transformation Functions
	inline const Vector3& GetPosition() const { return zPos; }
	inline const Vector3& GetPreviousPos() const { return zPreviousPos; }
	inline const Vector4& GetRotation() const { return zRot; }
	inline const Vector3& GetScale() const { return zScale; }
	inline const Vector3& GetDir() const { return zDir; }
	virtual const std::string GetModel() const;
	PhysicsObject* GetPhysicsObject() {return this->zPhysicsObject;}
	inline unsigned int GetType() const {return this->zType;}
	inline float GetEnergy() {return this->zEnergy;}

	inline float GetCollisionRadius() const {return this->zCollisionRadius;}
	/*!Returns an array of CollisionPoints. There are 4 points.*/
	inline const Vector2* GetCollisionPoints() {return this->zCollisionPoints;}

};