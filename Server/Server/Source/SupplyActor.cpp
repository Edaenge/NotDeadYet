#include "SupplyActor.h"

SupplyActor::SupplyActor( const unsigned int inventoryCapacity )
{
	this->zInventory = new Inventory(inventoryCapacity);
	this->zParachute = NULL;
	this->zCollisionRadius = 0.5f;
}

SupplyActor::~SupplyActor()
{
	SAFE_DELETE(this->zInventory);
}

bool SupplyActor::AddItem( Item* item )
{
	bool stacked = false;
	return this->zInventory->AddItem(item, stacked);
}

Item* SupplyActor::DropItem( Item* item )
{
	return item = this->zInventory->RemoveItem(item);
}

Item* SupplyActor::DropItem( const unsigned int itemID )
{
	return this->zInventory->RemoveItem(itemID);
}

bool SupplyActor::HasParachute() const
{
	if( zParachute )
		return true;

	else
		 return false;
}

bool SupplyActor::AttachParachute( Actor* parachute )
{
	if( zParachute )
		return false;

	zParachute = parachute;

	return true;
}

Actor* SupplyActor::DetatchParachute()
{
	Actor* temp = zParachute;
	zParachute = NULL;

	return temp;
}