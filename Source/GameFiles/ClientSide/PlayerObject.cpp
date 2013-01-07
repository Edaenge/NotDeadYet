#include "GameFiles/ClientSide/PlayerObject.h"

PlayerObject::PlayerObject(const unsigned int id) : OrganicObject(id)
{
	this->zHunger = 0.0f;
	this->zStamina = 0.0f;
	this->zHydration = 0.0f;
	this->zEquipment = new Equipment();
}

void PlayerObject::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();

	float t = this->GetInterpolationType(deltaTime, IT_SMOOTH_STEP);
	
	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetPosition(currentPosition);
}