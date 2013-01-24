#include "Food.h"

Food::Food(const long id, const unsigned int TYPE, const float hunger)
	: Item(id, TYPE)
{
	this->zHunger = hunger;
	this->zStacking = true;
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