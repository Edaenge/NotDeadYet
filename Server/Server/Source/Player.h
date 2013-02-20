#pragma once

#include <Observer.h>
#include <KeyStates.h>
#include "Observer.h"

class ClientData;
class Behavior;


class Player : public Observed
{
	// The Client Data
	ClientData* zClient;

	// Current Key States
	KeyStates zKeys;

	// Pointer Set By Game Class
	Behavior* zBehavior;

	//The player Name
	std::string zUserName;

private:
	Player( ClientData* client );
	virtual ~Player();

public:
	// Players Current Key States
	inline KeyStates& GetKeys() { return zKeys; }

	// Disconnect This Player
	void Kick();

	// Returns The Current Behavior Of Player
	Behavior* GetBehavior() const { return zBehavior; }

	//Returns Client Data
	ClientData* GetClientData() const {return this->zClient;}

	//Returns player name
	std::string GetPlayerName() const {return this->zUserName;}

private:
	friend class Game;
};