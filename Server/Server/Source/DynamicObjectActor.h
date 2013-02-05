/*
Made by Edänge Simon Datum(19/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "PhysicsActor.h"
#include "PhysicsObject.h"

class DynamicObjectActor : public PhysicsActor
{
public:
	DynamicObjectActor(bool genID = false);
	DynamicObjectActor(const std::string& meshModel, bool genID = false);
	DynamicObjectActor(const std::string& meshModel, int objOwner, bool genID = false);
	virtual ~DynamicObjectActor();

	virtual void Update(float deltaTime) = 0;

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	int GetSubType() const {return this->zSubType;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}
	int GetStackSize() const {return this->zStackSize;}
	/*! Returns the ID of the player who created this object.
		Returns -1 if this object has no owner.
	*/
	int GetObjPlayerOwner(){return this->zObjPlayerOwner;}

	/*! Sets the player owner of this object.*/
	void SetObjOwner(int ID){this->zObjPlayerOwner = ID;}
	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int Type) {this->zType = Type;}
	void SetType(const int Type) {this->zSubType = Type;}
	void SetStackSize(const int size) {this->zStackSize = size;}
	void SetMoving(const bool op){this->zMoving = op;}

	bool IsMoving() const {return this->zMoving;}
	void ModifyStackSize(const int size) {this->zStackSize += size;}

private:
	void InitValues();

protected:
	int zType;
	int zSubType;
	int zWeight;
	int zStackSize;
	int zObjPlayerOwner;
	bool zMoving;
	std::string zDescription;
	std::string zIconPath;
	
private:
};
