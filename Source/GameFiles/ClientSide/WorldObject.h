#pragma once

#include "iMesh.h"

class WorldObject
{
public:
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetPlayerMesh() const {return this->zMesh;}
	/*! Returns Player Model Scale*/
	Vector3 GetPlayerScale() const {return this->zMesh->GetScaling();}
	/*! Returns Player Model Position*/
	inline Vector3 GetPlayerPosition() const {return this->zMesh->GetPosition();}
	/*! Returns Player Model Rotation*/
	inline Vector3 GetPlayerRotation() const {return this->zMesh->GetRotation();}

	virtual void Update(float dt) = 0;
private:
	iMesh* zMesh;
	int zId;
};