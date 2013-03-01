#include "SupplyActor.h"

SupplyActor::SupplyActor( const unsigned int inventoryCapacity )
{
	this->zInventory = new Inventory(inventoryCapacity);
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

