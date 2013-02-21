#include "ItemActor.h"

ItemActor::ItemActor(Item* item) : Actor()
{
	this->zItem = item;
}

ItemActor::~ItemActor()
{
	SAFE_DELETE(this->zItem);
}

const std::string& ItemActor::GetModel() const
{
	if(zItem)
		return this->zItem->GetModel();

	static const std::string emptyString = "";

	return emptyString;
}