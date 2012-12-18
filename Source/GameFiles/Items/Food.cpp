#include "GameFiles/Items/Food.h"

Food::Food(const unsigned int id, const unsigned int weight, const std::string& name, 
		   const unsigned int itemType, const float hunger)
	: Item(id, weight, name, itemType)
{
	this->zHunger = hunger;
}

Food::~Food()
{

}

void Food::Use()
{

}