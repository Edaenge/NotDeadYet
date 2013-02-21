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
	/*! Sets Item pointer to NULL.*/
	inline void RemoveItem() {this->zItem = NULL;}
	virtual const std::string& GetModel() const;

private:
	Item* zItem;
};