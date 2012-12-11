#include "GameFiles/Inventory/Inventory.h"

Inventory::Inventory()
{
	this->zInventoryCap  = 10;
	for (unsigned int i = 0; i < this->zInventoryCap; i++)
	{
		this->zInventorySlotUsed[i] = false;
	}
	this->zSlotsAvailable = this->zInventoryCap;
}

Inventory::~Inventory()
{
	//Remove Item Array
	for (auto x = this->zItems.begin(); x < this->zItems.end(); x++)
	{
		delete (*x);
	}
}

const std::vector<Item*>& Inventory::GetItems()
{
	return this->zItems;
}

bool Inventory::AddItem(Item* item)
{
	int weight = item->GetWeight();
	if(this->zItems.size() + weight < this->zInventoryCap)
	{
		if (zSlotsAvailable > weight)
		{
			this->zWeightTotal += weight;
			for (int i = 0; i < weight; i++)
			{
				zInventorySlotUsed[this->zSlotsAvailable--] = true;
			}
			this->zItems.push_back(item);

			return true;
		}
	}
	return false;
}

Item* Inventory::SearchAndGetItem(unsigned int ID)
{
	int position = this->Search(ID);

	return this->GetItem(position);
}

Item* Inventory::GetItem(unsigned int position)
{
	if (position < this->zItems.size())
	{
		return this->zItems[position];
	}
	return 0;
}

int Inventory::Search(const unsigned int ID)
{
	for (unsigned int i = 0; i < this->zItems.size(); i++)
	{
		if (this->zItems[i])
		{
			if (ID == this->zItems[i]->GetID())
				return i;
		}
	}
	return -1;
}

bool Inventory::RemoveItem(const unsigned int position)
{
	if (position < this->zItems.size() && position > -1)
	{
		int weight = this->zItems[position]->GetWeight();
		this->zWeightTotal -= weight;
		for (int i = 0; i < zSlotsAvailable + weight; i++)
		{
			this->zInventorySlotUsed[i] = false;
		}
		this->zItems.erase(this->zItems.begin() + position);

		return true;
	}
	return false;
}

bool Inventory::RemoveItem(Item* item)
{
	int position = this->Search(item->GetID());

	return this->RemoveItem(position);
}
