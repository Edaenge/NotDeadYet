/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "stdafx.h"
#include "StaticMesh.h"

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
	/*! Returns the Client ID */
	int GetClientID();
	/*! Returns Current Player State */
	int GetPlayerState();
	/*!	Returns Pointer to the Player Mesh*/
	StaticMesh* GetPlayerMesh();
	/*! Returns Player Model Scale */
	D3DXVECTOR3 GetPlayerScale();
	/*! Returns Player Model Position */
	D3DXVECTOR3 GetPlayerPosition();
	/*! Returns Player Model Rotation */
	D3DXQUATERNION GetPlayerRotation();
	/*! Adds a Pointer to the Player Model Mesh */
	void AddStaticMesh(StaticMesh* mesh);
	/*!  */
	void SetClientID(int clientID);
private:
	StaticMesh* zPlayerMesh;
	int zPlayerState;
	int zClientID;
};