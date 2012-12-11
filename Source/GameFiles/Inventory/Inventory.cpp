#include "GameFiles/Inventory/Inventory.h"
#include "Safe.h"

Inventory::Inventory()
{
	this->zItemCap  = 10;
}

Inventory::~Inventory()
{
	//Remove Item Array
	for (auto x = this->zItems.begin(); x < this->zItems.end(); x++)
	{
		delete (*x);
	}
}

std::vector<Item*> Inventory::GetItems()
{
	return this->zItems;
}

bool Inventory::AddItem(Item* item)
{
	if(this->zItems.size() < this->zInventoryCap)
	{
		this->zWeightTotal += item->GetWeight();
		this->zItems.push_back(item);

		return true;
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

int Inventory::Search(unsigned int ID)
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

bool Inventory::RemoveItem(unsigned int position)
{
	if (position < this->zItems.size() && position > -1)
	{
		this->zWeightTotal -= this->zItems[position]->GetWeight();
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
