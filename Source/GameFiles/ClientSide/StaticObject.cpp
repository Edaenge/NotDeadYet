#include "GameFiles/ClientSide/StaticObject.h"

StaticObject::StaticObject(const unsigned int id) : WorldObject(id)
{
	this->zType = 0;
	this->zWeight = 0;
	this->zName = "Unknown";
	this->zIconPath = "Unknown";
	this->zDescription = "Unknown";
}