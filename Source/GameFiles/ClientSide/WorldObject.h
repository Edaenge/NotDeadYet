/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "iMesh.h"
/*! Base class for World Objects*/
class WorldObject
{
public:
	WorldObject(const unsigned int id){this->zMesh = 0; this->zID = id;}
	virtual ~WorldObject(){}
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetMesh() const {return this->zMesh;}
	/*! Returns Object Model Scale*/
	Vector3 GetScale() const {return this->zMesh->GetScaling();}
	/*! Returns Object Model Position*/
	inline Vector3 GetPosition() const {return this->zMesh->GetPosition();}
	/*! Returns Object Model Rotation*/
	inline Vector4 GetRotation() const {return this->zMesh->GetRotationQuaternion();}
	/*! Sets object Mesh data*/
	inline void SetPosition(const Vector3& pos) {this->zMesh->SetPosition(pos);}
	void SetScale(const Vector3& scale) {this->zMesh->Scale(scale);}
	inline void SetRotation(const Vector4& rot) {this->zMesh->SetQuaternion(rot);}
	/*! Returns the object ID*/
	inline unsigned int GetID() const {return this->zID;}
	/*! Adds a Pointer to the Player Model Mesh*/
	void SetStaticMesh(iMesh* mesh) {this->zMesh = mesh;}
	bool HasMesh() const 
	{
		if(this->zMesh != 0) 
			return true;
		else 
			return false;
	}
	/*!  Sets the Client Id given from the server*/
	void SetID(const int clientID) {this->zID = clientID;}

protected:
	iMesh* zMesh;
	unsigned int zID;
};