#include "Inventory.h"

Inventory::Inventory()
{
	this->zInventoryCap = 20;
	this->zWeightTotal = 0;
	
	this->zInventorySlotBlocked = std::vector<bool>();
	this->zItems = std::vector<Item*>();

	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotBlocked.push_back(false);
	}
	this->zSlotsAvailable = this->zInventoryCap;
}

Inventory::Inventory(const unsigned int inventorySize)
{
	this->zInventoryCap = inventorySize;
	this->zWeightTotal = 0;

	this->zInventorySlotBlocked = std::vector<bool>();
	this->zItems = std::vector<Item*>();

	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotBlocked.push_back(false);
	}
	this->zSlotsAvailable = this->zInventoryCap;
}

Inventory::~Inventory()
{
	//Remove Item Arrays
	for (auto x = this->zItems.begin(); x < this->zItems.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}
}

std::vector<Item*> Inventory::GetItems() const
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
			zInventorySlotBlocked[this->zSlotsAvailable-1] = true;

			//Decrease slot size to mark blocked
			this->zSlotsAvailable--;
		}
		this->zItems.push_back(item);
		MaloW::Debug("Added Item "+item->GetItemName() + MaloW::convertNrToString(item->GetID()));
		return true;
	}

	MaloW::Debug("Failed to add item "+item->GetItemName() + MaloW::convertNrToString(item->GetID()));
	return false;
}

Item* Inventory::SearchAndGetItem(unsigned int ID) const
{
	int position = this->Search(ID);

	return this->GetItem(position);
}

Item* Inventory::GetItem(const unsigned int index) const
{
	if (index < this->zItems.size())
	{
		return this->zItems[index];
	}
	return 0;
}

int Inventory::Search( const int ID ) const
{
	for (unsigned int i = 0; i < this->zItems.size(); i++)
	{
		Item* temp = this->zItems[i];
		if (temp)
		{
			if (ID == temp->GetID())
				return i;
		}
	}
	return -1;
}

bool Inventory::RemoveItem(const unsigned int index)
{
	if (index < this->zItems.size())
	{
		int weight = GetItem(index)->GetWeight();
		this->zWeightTotal -= weight;

		for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}
		Item* item = this->zItems.at(index);
		this->zItems.erase(this->zItems.begin() + index);

		if (item)
		{
			delete item;
			item = NULL;
		}
		return true;
	}
	return false;
}

bool Inventory::RemoveItem(Item* item)
{
	int index = this->Search(item->GetID());
	if (index != -1)
	{
		return this->RemoveItem(index);
	}
	return false;
}

std::vector<bool> Inventory::GetBlockedSlots() const
{
	return this->zInventorySlotBlocked;
}

bool Inventory::GetBlockedSlot(unsigned int index) const
{
	return this->zInventorySlotBlocked[index];
}

int Inventory::GetInventoryCapacity() const
{
	return this->zInventoryCap;
}

int Inventory::SearchForItemType(const int TYPE)
{
	int counter = 0;

	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == TYPE)
		{
			counter++;
		}
	}

	return counter;
}
