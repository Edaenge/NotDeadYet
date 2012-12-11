#include "GameFiles/Inventory/Inventory.h"

Inventory::Inventory()
{
	this->zInventoryCap = 20;
	this->zWeightTotal = 0;
	
	this->zInventorySlotBlocked = new MaloW::Array<bool>();
	this->zItems = new MaloW::Array<Item*>();

	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotBlocked->add(false);
	}
	this->zSlotsAvailable = this->zInventoryCap;
}

Inventory::~Inventory()
{
	//Remove Item Arrays
	while (this->zItems->size() > 0)
	{
		delete this->zItems->getAndRemove(0);
	}
	delete this->zItems;
	this->zItems = 0;


	if (this->zInventorySlotBlocked)
	{
		delete this->zInventorySlotBlocked;
		this->zInventorySlotBlocked = 0;
	}
	
}

MaloW::Array<Item*>* Inventory::GetItems() const
{
	return this->zItems;
}

bool Inventory::AddItem(Item* item)
{
	int weight = item->GetWeight();
	if(this->zWeightTotal + weight <= this->zInventoryCap)
	{
		this->zWeightTotal += weight;
		for (int i = 0; i < weight - 1; i++)
		{
			zInventorySlotBlocked->get(this->zSlotsAvailable-1) = true;

			//Decrease slot size to mark blocked
			this->zSlotsAvailable--;
		}
		this->zItems->add(item);

		return true;
	}
	return false;
}

Item* Inventory::SearchAndGetItem(unsigned int ID) const
{
	int position = this->Search(ID);

	return this->GetItem(position);
}

Item* Inventory::GetItem(const unsigned int position) const
{
	if (position < this->zItems->size())
	{
		return this->zItems->get(position);
	}
	return 0;
}

int Inventory::Search(const unsigned int ID) const
{
	for (int i = 0; i < this->zItems->size(); i++)
	{
		Item* temp = this->zItems->get(i);
		if (temp)
		{
			if (ID == temp->GetID())
				return i;
		}
	}
	return -1;
}

bool Inventory::RemoveItem(const unsigned int position)
{
	if (position < this->zItems->size())
	{
		int weight = GetItem(position)->GetWeight();
		this->zWeightTotal -= weight;

		for (int i = 0; i < weight; i++)
		{
			this->zInventorySlotBlocked->get(zSlotsAvailable++) = false;
		}

		Item* item = this->zItems->getAndRemove(position);
		delete item;
		item = 0;

		return true;
	}
	return false;
}

bool Inventory::RemoveItem(Item* item)
{
	int position = this->Search(item->GetID());
	if (position != -1)
	{
		return this->RemoveItem(position);
	}
	return false;
}

MaloW::Array<bool>* Inventory::GetBlockedSlots() const
{
	return this->zInventorySlotBlocked;
}

bool Inventory::GetBlockedSlot(unsigned int position) const
{
	return this->zInventorySlotBlocked->get(position);
}

int Inventory::GetInventoryCapacity() const
{
	return this->zInventoryCap;
}
