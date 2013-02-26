/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "iMesh.h"
#include <AnimationStates.h>
#include <string>
#include "Safe.h"

/*! Base class for World Objects*/
class Actor
{
public:
	Actor(const unsigned int ID)
	{
		this->zMesh = 0; 
		this->zID = ID;
	}
	virtual ~Actor(){}
	std::string GetModel() {return this->zModel;}
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetMesh() const {return this->zMesh;}
	/*! Returns Object Model Scale*/
	Vector3 GetScale() const {return this->zMesh->GetScaling();}
	/*! Returns Object Model Position*/
	inline Vector3 GetPosition() const { return this->zMesh->GetPosition();}
	/*! Returns Object Model Rotation*/
	inline Vector4 GetRotation() const {return this->zMesh->GetRotationQuaternion();}
	
	/*! Sets Actor Mesh data*/
	void SetModel(std::string model) {this->zModel = model;}
	inline void SetPosition(const Vector3& pos) 
	{
		if (this->zMesh)
			this->zMesh->SetPosition(pos);
	}
	void SetScale(const Vector3& scale) {this->zMesh->SetScale(scale);}
	inline void SetRotation(const Vector4& rot) 
	{
		this->zMesh->ResetRotation();
		this->zMesh->SetQuaternion(rot);
	}
	/*! Returns the object ID*/
	inline unsigned int GetID() const {return this->zID;}
	/*! Adds a Pointer to the Player Model Mesh*/
	void SetStaticMesh(iMesh* mesh) {this->zMesh = mesh;}
	/*!  Sets the Client Id given from the server*/
	void SetID(const int clientID) {this->zID = clientID;}

protected:
	std::string zModel;
	iMesh* zMesh;
	unsigned int zID;
	Vector3 zPosition;
};