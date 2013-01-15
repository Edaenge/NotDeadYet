#include "MediumStick.h"

MediumStick::MediumStick(const unsigned int id, const unsigned int itemType) 
	: Material(id, itemType)
{

}

MediumStick::~MediumStick()
{

}

bool MediumStick::Use()
{
	if (this->zStacks > 2)
	{
		return true;
	}
	return false;
}