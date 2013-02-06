#include "ItemActor.h"

ItemActor::ItemActor(Item* item, const std::string& model, const Vector3& position)
{
	this->zItem = item;
	this->zActorModel = model;
	this->SetPosition(position);
}

ItemActor::~ItemActor()
{
	
}