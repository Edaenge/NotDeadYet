#include "LargeStick.h"

LargeStick::LargeStick(const unsigned int id, const unsigned int itemType)
	: Material(id, itemType)
{
	
}

LargeStick::~LargeStick()
{

}

bool LargeStick::Use()
{
	if (this->zStacks > 2)
	{
		return true;
	}
	return false;
}