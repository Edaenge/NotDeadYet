#pragma once

#include <KeyStates.h>
#include <Observer.h>

class ClientData;

class Player : public Observed
{
	ClientData* zClient;
	KeyStates zKeys;
public:
	Player( ClientData* client );
	virtual ~Player();

	// Players Current Key States
	inline KeyStates& GetKeys() { return zKeys; }

	// Disconnect This Player
	void Kick();
};