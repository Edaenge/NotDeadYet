//Simon Edänge 2013-02-27
#pragma once

#include "Actor.h"
#include "Inventory.h"

class SupplyActor : public Actor
{
public:
	SupplyActor( const unsigned int inventoryCapacity );
	virtual ~SupplyActor();

	bool AddItem( Item* item );
	bool IsEmpty() { return this->zInventory->GetItems().empty(); }

	Item* DropItem( Item* item );
	Item* DropItem( const unsigned int itemID );

	bool HasParachute() const;
	bool AttachParachute(Actor* parachute);
	Actor* DetatchParachute();
	inline Actor* GetParachute() const {return zParachute;}
	inline Inventory* GetInventory() const {return this->zInventory;}
	
private:
	Inventory* zInventory;
	Actor* zParachute;
};