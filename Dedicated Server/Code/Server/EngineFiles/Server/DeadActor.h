/*
	Made by Simon Edänge Datum(31/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Actor.h"
#include "Item.h"
#include <vector>

/*
This class is used to create dead BioActors in the world.
This class is abstract.
*/
class DeadActor : public Actor
{
public:
	DeadActor(const bool genID = false);
	virtual ~DeadActor();

	inline void SetPosition(const Vector3& pos) {zPos = pos;}
	inline void SetRotation(const Vector4& rot) {zRot = rot;}
	void SetScale(const Vector3& scale) {zScale = scale;}

	Item* GetItemAt(const int Index) const;
	Item* GetItem(const long ID) const;
	std::vector<Item*> GetItems() const {return this->zItems;}

	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}

	virtual void Update(float deltaTime) = 0;

	/*! Returns Pos, Rot, Scale.*/
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);

protected:
	std::vector<Item*> zItems;
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
};