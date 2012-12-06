/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include <iMesh.h>
#include "GameFiles/AnimationStates.h"


//Keeps track of player Info
class Player
{
public:
	Player();
	virtual ~Player();
	/*! Returns the Client ID*/
	inline int GetClientID() const {return this->zClientID;}
	/*! Returns Current Player State*/
	inline int GetPlayerState() const {return this->zPlayerState;}
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetPlayerMesh() const;
	/*! Returns Player Model Scale*/
	Vector3 GetPlayerScale() const;
	/*! Returns Player Model Position*/
	Vector3 GetPlayerPosition() const;
	/*! Returns Player Model Rotation*/
	Vector3 GetPlayerRotation() const;
	/*! Adds a Pointer to the Player Model Mesh*/
	void AddStaticMesh(iMesh* mesh);
	/*!  Sets the Client Id given from the server*/
	inline void SetClientID(const int clientID) {this->zClientID = clientID;}
	/*! Sets the Player State*/
	inline void SetPlayerState(const int state) {this->zPlayerState = state;}
	/*! Sets Player Mesh data*/
	void SetPlayerPosition(const Vector3& pos);
	void SetPlayerScale(const Vector3& scale);
	void SetPlayerRotation(const Vector4& rot);
private:
	iMesh* zPlayerMesh;
	int zPlayerState;
	int zClientID;
};