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

public:
	Player( ClientData* client );
	virtual ~Player();

	// Players Current Key States
	inline KeyStates& GetKeys() { return zKeys; }

	// Disconnect This Player
	void Kick();

	// Returns The Current Behavior Of Player
	Behavior* GetBehavior() const { return zBehavior; }

private:
	friend class Game;
};