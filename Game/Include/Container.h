/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_CONTAINER_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_CANTEEN,
	ITEM_SUB_TYPE_WATER_BOTTLE
};

class Container : public Item
{
public:
	Container(const unsigned int id, const unsigned int itemType, const unsigned int itemSubType,
		const int maxUse, const int currUse);
	Container(const Container& other);
	Container(const Container* other);
	virtual ~Container();

	void SetMaxUses(const int size) {this->zMaxUses = size;}
	void SetRemainingUses(const int size) {this->zUsesRemaining = size;}

	inline int GetRemainingUses() {return this->zUsesRemaining;}
	inline int GetMaxUses() {return this->zMaxUses;}

	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC); 
private:
	int zMaxUses;
	int zUsesRemaining;
};