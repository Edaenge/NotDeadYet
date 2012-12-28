/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Item.h"

class Gear : public Item
{
public:
	Gear(const unsigned int id, const unsigned int weight, const std::string& name, 
		const unsigned int itemType, const std::string& description);
	virtual ~Gear();
	void Use();
private:

};