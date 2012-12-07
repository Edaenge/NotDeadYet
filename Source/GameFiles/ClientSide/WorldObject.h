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
	WorldObject(){this->zMesh = 0; this->zID = -1;}
	~WorldObject(){}
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetObjectMesh() const {return this->zMesh;}
	/*! Returns Player Model Scale*/
	Vector3 GetObjectScale() const {return this->zMesh->GetScaling();}
	/*! Returns Player Model Position*/
	inline Vector3 GetObjectPosition() const {return this->zMesh->GetPosition();}
	/*! Returns Player Model Rotation*/
	inline Vector3 GetObjectRotation() const {return this->zMesh->GetRotation();}
	/*! Sets object Mesh data*/
	inline void SetObjectPosition(const Vector3& pos) {this->zMesh->SetPosition(pos);}
	void SetObjectScale(const Vector3& scale) {this->zMesh->Scale(scale);}
	inline void SetObjectRotation(const Vector4& rot) {this->zMesh->SetQuaternion(rot);}
	/*! Returns the object ID*/
	inline int GetID() const {return this->zID;}
	/*! Adds a Pointer to the Player Model Mesh*/
	void AddStaticMesh(iMesh* mesh) {this->zMesh = mesh;}
	bool HasMesh() const 
	{
		if(this->zMesh != 0) 
			return true;
		else 
			return false;
	}
	/*!  Sets the Client Id given from the server*/
	inline void SetID(const int clientID) {this->zID = clientID;}

	virtual void Update(float dt) = 0;
protected:
	iMesh* zMesh;
	int zID;
};