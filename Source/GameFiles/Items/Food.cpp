#include "Food.h"

Food::Food(const unsigned int id, const unsigned int weight, const std::string& name, 
		   const unsigned int itemType, const std::string& description, const float hunger)
	: Item(id, weight, name, itemType, description)
{
	this->zHunger = hunger;
}

Food::~Food()
{

}

bool Food::Use()
{
	if (this->zStacks > 0)
	{
		this->zStacks--;
		return true;
	}
	return false;
}