#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor(bool genID) : Actor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zStacks = 1;
	this->zScale = Vector3(0.5f,0.5f,0.5f);
	this->zPos = Vector3(0,1,0);
	this->zScaleChanged = true;
}

StaticObjectActor::StaticObjectActor(const std::string& meshModel, bool genID) : Actor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zActorModel = meshModel;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zStacks = 1;
	this->zScale = Vector3(0.5f,0.5f,0.5f);
	this->zPos = Vector3(0,1,0);
	this->zScaleChanged = true;
}

StaticObjectActor::~StaticObjectActor()
{
}

std::string StaticObjectActor::ToMessageString( NetworkMessageConverter* NMC, bool getDataOnly /*= false*/ )
{
	std::string mess;

	if(!getDataOnly)
	{
		mess = NMC->Convert(MESSAGE_TYPE_POSITION, this->zPos.x, this->zPos.y, this->zPos.z);
		mess += NMC->Convert(MESSAGE_TYPE_ROTATION, this->zRot.x, this->zRot.y, this->zRot.z, this->zRot.w );

		if(zScaleChanged)
		{
			mess +=  NMC->Convert(MESSAGE_TYPE_SCALE, this->zScale.x, this->zScale.y, this->zScale.z);
			zScaleChanged = false;
		}
	}

	mess += NMC->Convert(MESSAGE_TYPE_ITEM_ICON_PATH, this->zIconPath);
	mess += NMC->Convert(MESSAGE_TYPE_ITEM_NAME, this->zActorObjectName);
	mess += NMC->Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, this->zDescription);
	mess += NMC->Convert(MESSAGE_TYPE_ITEM_WEIGHT, this->zWeight);
	mess += NMC->Convert(MESSAGE_TYPE_ITEM_TYPE, this->zType);
	mess += NMC->Convert(MESSAGE_TYPE_MESH_MODEL, this->zActorModel);

	return mess;
}