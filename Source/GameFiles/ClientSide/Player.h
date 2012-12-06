/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include <iMesh.h>
#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/WorldObject.h"

//Keeps track of player Info
class Player : public WorldObject
{
public:
	Player(){this->zPlayerState = IDLE; this->zPlayerMesh = 0;}
	virtual ~Player(){}
	/*! Returns the Client ID*/
	inline int GetClientID() const {return this->zClientID;}
	/*! Returns Current Player State*/
	inline int GetPlayerState() const {return this->zPlayerState;}
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetPlayerMesh() const {return this->zPlayerMesh;}
	/*! Returns Player Model Scale*/
	Vector3 GetPlayerScale() const {return this->zPlayerMesh->GetScaling();}
	/*! Returns Player Model Position*/
	inline Vector3 GetPlayerPosition() const {return this->zPlayerMesh->GetPosition();}
	/*! Returns Player Model Rotation*/
	inline Vector3 GetPlayerRotation() const {return this->zPlayerMesh->GetRotation();}
	/*! Adds a Pointer to the Player Model Mesh*/
	void AddStaticMesh(iMesh* mesh) {this->zPlayerMesh = mesh;}
	/*!  Sets the Client Id given from the server*/
	inline void SetClientID(const int clientID) {this->zClientID = clientID;}
	/*! Sets the Player State*/
	inline void SetPlayerState(const int state) {this->zPlayerState = state;}
	/*! Sets Player Mesh data*/
	inline void SetPlayerPosition(const Vector3& pos) {this->zPlayerMesh->SetPosition(pos);}
	void SetPlayerScale(const Vector3& scale) {this->zPlayerMesh->Scale(scale);}
	inline void SetPlayerRotation(const Vector4& rot) {this->zPlayerMesh->SetQuaternion(rot);}
	void Update(float dt){};
private:
	iMesh* zPlayerMesh;
	int zPlayerState;
	int zClientID;
};