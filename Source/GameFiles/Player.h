/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include <iMesh.h>

/*! Animation states*/
enum PlayerState
{
	IDLE,
	WALKING,
	RUNNING,
	CROUCHING
};
//Keeps track of player Info
class Player
{
public:
	Player();
	virtual ~Player();
	/*! Returns the Client ID*/
	const int GetClientID();
	/*! Returns Current Player State*/
	const int GetPlayerState();
	/*!	Returns Pointer to the Player Mesh*/
	iMesh* GetPlayerMesh();
	/*! Returns Player Model Scale*/
	const Vector3 GetPlayerScale();
	/*! Returns Player Model Position*/
	const Vector3 GetPlayerPosition();
	/*! Returns Player Model Rotation*/
	const Vector3 GetPlayerRotation();
	/*! Adds a Pointer to the Player Model Mesh*/
	void AddStaticMesh(iMesh* mesh);
	/*!  Sets the Client Id given from the server*/
	void SetClientID(const int clientID);
	/*! Sets the Player State*/
	void SetPlayerState(const int state);
	/*! Sets Player Mesh data*/
	void SetPlayerPosition(Vector3 pos);
	void SetPlayerScale(Vector3 scale);
	void SetPlayerRotation(Vector4 rot);
private:
	iMesh* zPlayerMesh;
	int zPlayerState;
	int zClientID;
};