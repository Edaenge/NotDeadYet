#include "Inventory.h"
#include "../../MaloWLib/Safe.h"
#include "../ClientServerMessages.h"
#include "../../MaloWLib/MaloWFileDebug.h"

Inventory::Inventory()
{
	this->zInventoryCap = 49;
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
		SAFE_DELETE((*x));

	}
}

std::vector<Item*> Inventory::GetItems() const
{
	return this->zItems;
}

bool Inventory::AddItem(Item* item)
{
	if (!item)
		return false;

	unsigned int weight = item->GetWeight() * item->GetStackSize();
	if(this->zWeightTotal + weight <= this->zInventoryCap)
	{
		this->zWeightTotal += weight;
		//for (unsigned int i = 0; i < weight - 1; i++)
		//{
			//zInventorySlotBlocked[this->zSlotsAvailable-1] = true;

			//Decrease slot size to mark blocked
			//this->zSlotsAvailable--;
		//}
		if (item->GetStacking())
		{
			Item* existingItem = this->SearchAndGetItemFromType(item->GetItemType());
			if (existingItem)
			{
				if (existingItem->GetItemType() == item->GetItemType())
				{
					existingItem->IncreaseStackSize(item->GetStackSize());
					if (Messages::FileWrite())
						Messages::Debug("Added Stack to inventory " + item->GetItemName());

					return true;
				}
			}
		}
		this->zItems.push_back(item);

		if (Messages::FileWrite())
			Messages::Debug("Added Item " + item->GetItemName() + " ID: " + MaloW::convertNrToString((float)item->GetID()));

		return true;
	}

	MaloW::Debug("Failed to add item "+item->GetItemName() + " ID: " +MaloW::convertNrToString((float)item->GetID()));
	return false;
}

Item* Inventory::SearchAndGetItem(long ID) const
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

int Inventory::Search(const long ID) const
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

bool Inventory::RemoveItemStack(const long ID, const unsigned int numberOfStacks)
{
	int index = this->Search(ID);
	if (index == -1)
		return false;

	if ((unsigned int)index < this->zItems.size())
	{
		int weight = GetItem(index)->GetWeight() * numberOfStacks;
		this->zWeightTotal -= weight;

		/*for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}*/
		if (Messages::FileWrite())
		{
			Messages::Debug("Removed Stacks: " + MaloW::convertNrToString((float)numberOfStacks) + " TotalWeight: " + MaloW::convertNrToString((float)zWeightTotal));
		}
		return true;
	}
	return false;
}

bool Inventory::RemoveItem(const unsigned int index)
{
	if (index < this->zItems.size())
	{
		int weight = GetItem(index)->GetWeight();
		this->zWeightTotal -= weight;

		/*for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}*/
		Item* item = this->zItems.at(index);
		this->zItems.erase(this->zItems.begin() + index);

		SAFE_DELETE((item));

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

int Inventory::SearchForItemType(const unsigned int TYPE)
{
	int counter = 0;

	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == TYPE)
		{
			counter += (*it)->GetStackSize();
		}
	}

	return counter;
}

Item* Inventory::SearchAndGetItemFromType( const unsigned int TYPE )
{
	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == TYPE)
		{
			return (*it);
		}
	}
	return NULL;
}

Item* Inventory::EraseItem(const long ID)
{
	int index = this->Search(ID);

	if (index == -1)
		return NULL;
	
	if ((unsigned int)index < this->zItems.size())
	{
		Item* item = GetItem(index);
		int weight = item->GetWeight() * item->GetStackSize();
		this->zWeightTotal -= weight;

		/*for (int i = 0; i < weight - 1; i++)
		{
			this->zInventorySlotBlocked[zSlotsAvailable++] = false;
		}*/
		this->zItems.erase(this->zItems.begin() + index);

		return item;
	}
	return NULL;
}