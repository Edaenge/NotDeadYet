#include "GameFiles/Items/Inventory.h"

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
		for (unsigned int i = 0; i < weight - 1; i++)
		{
			zInventorySlotBlocked[this->zSlotsAvailable-1] = true;

			//Decrease slot size to mark blocked
			this->zSlotsAvailable--;
		}
		this->zItems.push_back(item);

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
	if (position < this->zItems.size())
	{
		return this->zItems[position];
	}
	return 0;
}

int Inventory::Search(const unsigned int ID) const
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

bool Inventory::RemoveItem(const unsigned int position)
{
	if (position < this->zItems.size())
	{
		int weight = GetItem(position)->GetWeight();
		this->zWeightTotal -= weight;

		for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}

		this->zItems.erase(this->zItems.begin() + position);

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

std::vector<bool> Inventory::GetBlockedSlots() const
{
	return this->zInventorySlotBlocked;
}

bool Inventory::GetBlockedSlot(unsigned int position) const
{
	return this->zInventorySlotBlocked[position];
}

int Inventory::GetInventoryCapacity() const
{
	return this->zInventoryCap;
}