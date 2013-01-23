#include "GameFiles/ClientSide/StaticObject.h"

StaticObject::StaticObject(const long id) : WorldObject(id)
{
	this->zType = 0;
	this->zWeight = 0;
	this->zStackSize = 1;
	this->zName = "Unknown";
	this->zIconPath = "Unknown";
	this->zDescription = "Unknown";
}