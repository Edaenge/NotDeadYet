/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Item.h"

class Container : public Item
{
public:
	Container(const unsigned int id, const unsigned int weight, const std::string& name, 
		const unsigned int itemType, const std::string& itemDescription, const int maxUse, const int currUse);
	virtual ~Container();

	void SetMaxSize(const int size) {this->zMaxUses = size;}
	void SetCurrentSize(const int size) {this->zUsesRemaining = size;}

	inline int GetCurrentSize() {return this->zUsesRemaining;}
	inline float GetMaxSize() {return this->zMaxUses;}

	virtual void Use();
private:
	int zMaxUses;
	int zUsesRemaining;
};