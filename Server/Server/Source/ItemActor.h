/************************************/
/* Class that keep track of Objects */ 
/*      that can be Picked Up       */
/************************************/

#pragma once

#include "Actor.h"

class Item;

class ItemActor : public Actor
{
	Item* zItem;
public:

	ItemActor(Item* item);
	virtual ~ItemActor();

	inline Item* GetItem() const { return this->zItem; }
};