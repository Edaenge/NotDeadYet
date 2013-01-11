/*
Made by Edänge Simon Datum(19/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"


class DynamicObjectActor : public Actor
{
public:
	DynamicObjectActor(bool genID = false);
	DynamicObjectActor(const std::string& meshModel, bool genID = false);
	virtual ~DynamicObjectActor();

	virtual void Update(float deltaTime) = 0;

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}
	Vector3 GetDirection() const {return this->zDirection;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}
	void SetDirection(Vector3 dir) {this->zDirection = dir;}
protected:
	int zWeight;
	int zType;
	Vector3 zDirection;
	std::string zDescription;
	std::string zIconPath;
	
private:
};
