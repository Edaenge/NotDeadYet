/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "iMesh.h"
#include "Graphics.h"
#include <AnimationStates.h>
#include <string>
#include <map>
#include "Safe.h"

/*! Base class for World Objects*/
class Actor
{
	std::string zModel;
	iMesh* zMesh;

	//Used For FBX
	std::map<std::string, iMesh*> zSubMeshes;
	unsigned int zID;
	Vector3 zPosition;

public:
	Actor(const unsigned int& ID);
	virtual ~Actor();

	inline const std::string& GetModel() const { return this->zModel; }
	inline iMesh* GetMesh() const { return this->zMesh; }
	inline const Vector3& GetScale() const { return this->zMesh->GetScaling(); }
	inline const Vector3& GetPosition() const { return this->zMesh->GetPosition();}
	inline const Vector4& GetRotation() const { return this->zMesh->GetRotationQuaternion(); }
	inline const unsigned int& GetID() const { return this->zID; }

	/*! Sets Actor Mesh data*/
	void SetModel(const std::string& model);
	
	// Set Actor Position
	void SetPosition(const Vector3& pos);

	// Set Scale
	void SetScale(const Vector3& scale);
	
	// Set Rotation
	void SetRotation(const Vector4& rot);

	/*! Adds a Pointer to the Player Model Mesh*/
	void SetMesh(iMesh* mesh);

	/*! Sets the Client Id given from the server*/
	void SetID(const int clientID);

	// FBX bound meshes
	void AddSubMesh(const std::string& model, iMesh* mesh);
	iMesh* GetSubMesh(const std::string& model);
	void RemoveSubMesh(const std::string& model);
};