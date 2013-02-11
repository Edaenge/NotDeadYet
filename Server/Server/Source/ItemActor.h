/************************************/
/* Class that keep track of Objects */ 
/*      that can be Picked Up       */
/************************************/

#pragma once

#include "Actor.h"
#include "Item.h"

class ItemActor : public Actor
{
public:

	ItemActor(Item* item);
	virtual ~ItemActor();

	inline Item* GetItem() const { return this->zItem; }
	virtual const std::string& GetModel() const;

private:
	Item* zItem;

};