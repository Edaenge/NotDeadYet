/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include <string>

class Item
{
public:
	Item();
	Item(const int id);
	Item(const unsigned int id, const unsigned int weight, const std::string& name);
	virtual ~Item(){}
	void InitValues(unsigned int id = -1, unsigned int weight = 0, const std::string& name = "Unknown");
	/*! Returns Item ID*/
	inline unsigned int GetID() const {return this->zID;}
	/*! Returns the Item Weight*/
	inline unsigned int GetWeight() const {return this->zWeight;}
	/*! Sets the Item ID*/
	void SetID(const unsigned int id) {this->zID = id;}
	/*! Sets the Item Weight*/
	void SetWeight(const unsigned int weight) {this->zWeight = weight;}

	virtual void Use() = 0;
private:
	unsigned int zID;
	unsigned int zWeight;
	std::string zItemName;
};