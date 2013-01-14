
#include "Gear.h"

Gear::Gear(const unsigned int ID, const unsigned int TYPE) 
		   : Item(ID, TYPE)
{
	this->zStacking = false;
}

Gear::~Gear()
{

}

bool Gear::Use()
{
	return true;
}