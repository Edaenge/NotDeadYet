/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "stdafx.h"
#include "StaticMesh.h"

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
	StaticMesh* GetPlayerMesh();
	/*! Returns Player Model Scale*/
	const D3DXVECTOR3 GetPlayerScale();
	/*! Returns Player Model Position*/
	const D3DXVECTOR3 GetPlayerPosition();
	/*! Returns Player Model Rotation*/
	const D3DXQUATERNION GetPlayerRotation();
	/*! Adds a Pointer to the Player Model Mesh*/
	void AddStaticMesh(StaticMesh* mesh);
	/*!  Sets the Client Id given from the server*/
	void SetClientID(const int clientID);
	/*! Sets the Player State*/
	void SetPlayerState(const int state);
	/*! Sets Player Mesh data*/
	void SetPlayerPosition(D3DXVECTOR3 pos);
	void SetPlayerScale(D3DXVECTOR3 scale);
	void SetPlayerRotation(D3DXQUATERNION rot);
private:
	StaticMesh* zPlayerMesh;
	int zPlayerState;
	int zClientID;
};