/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class Container : public Item
{
public:
	Container(const unsigned int id, const unsigned int weight, const std::string& name, 
		const unsigned int itemType, const std::string& itemDescription, const int maxUse, const int currUse);
	virtual ~Container();

	void SetMaxUses(const int size) {this->zMaxUses = size;}
	void SetRemainingUses(const int size) {this->zUsesRemaining = size;}

	inline int GetRemainingUses() {return this->zUsesRemaining;}
	inline int GetMaxUses() {return this->zMaxUses;}

	virtual bool Use();
private:
	int zMaxUses;
	int zUsesRemaining;
};