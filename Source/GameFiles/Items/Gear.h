/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class Gear : public Item
{
public:
	Gear(const unsigned int ID, const unsigned int TYPE);
	virtual ~Gear();
	virtual bool Use();
private:

};