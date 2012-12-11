/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

class Item
{
public:
	Item(){this->zID = 0; this->zWeight = 0;}
	Item(const int id){this->zID = id; this->zWeight = 0;}
	Item(const unsigned int id, const unsigned int weight){this->zID = id; this->zWeight = weight;}
	virtual ~Item(){}
	/*! Returns Item ID*/
	unsigned int GetID() const {return this->zID;}
	/*! Returns the Item Weight*/
	unsigned int GetWeight() const {return this->zWeight;}
	/*! Sets the Item ID*/
	void SetID(const unsigned int id) {this->zID = id;}
	/*! Sets the Item Weight*/
	void SetWeight(const unsigned int weight) {this->zWeight = weight;}

private:
	unsigned int zID;
	unsigned int zWeight;
};