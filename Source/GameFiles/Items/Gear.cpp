
#include "Gear.h"

Gear::Gear(const long ID, const unsigned int TYPE) 
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