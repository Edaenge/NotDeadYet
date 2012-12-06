/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/WorldObject.h"

//Keeps track of player Info
class Player : public WorldObject
{
public:
	Player():WorldObject(){this->zPlayerState = IDLE;}
	virtual ~Player(){}
	/*! Sets the Player State*/
	inline void SetPlayerState(const int state) {this->zPlayerState = state;}
	/*! Returns Current Player State*/
	inline int GetPlayerState() const {return this->zPlayerState;}
	/*! Updates the play*/
	void Update(float dt){};
private:
	int zPlayerState;
};