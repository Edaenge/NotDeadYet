/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/MovingObject.h"
#include <string>

/*! Class to Handle Dynamic Objects*/
class DynamicObject : public MovingObject
{
public:
	DynamicObject(const long id);
	virtual ~DynamicObject(){}
	/*! Updates the Object*/
	virtual void Update(float deltaTime);

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	std::string GetName() const {return this->zName;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}
	int GetStackSize() const {return this->zStackSize;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetName(const std::string& name) {this->zName = name;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int type) {this->zType = type;}
	void SetStackSize(const int size) {this->zStackSize = size;}
	void ModifyStackSize(const int size) {this->zStackSize += size;}

private:
	int zType;
	int zWeight;
	int zStackSize;
	std::string zName;
	std::string zIconPath;
	std::string zDescription;
};