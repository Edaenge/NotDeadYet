#pragma once

#include <World/Observer.h>
#include <KeyStates.h>

class ClientData;
class Behavior;


class Player : public Observed, public Observer
{
	// The Client Data
	ClientData* zClient;

	// Current Key States
	KeyStates zKeys;

	// Pointer Set By Game Class
	Behavior* zBehavior;

	//The player Name
	std::string zUserName;

	//Player Model
	std::string zUserModel;

	//Ready state
	bool zReady;

private:
	Player( ClientData* client );
	virtual ~Player();

public:
	// Players Current Key States
	inline KeyStates& GetKeys() { return zKeys; }

	// Disconnect This Player
	void Kick();

	//
	void SetReady(bool flag) { this->zReady = flag;}
	// Returns The Current Behavior Of Player
	Behavior* GetBehavior() const { return zBehavior; }

	//Returns Client Data
	ClientData* GetClientData() const {return this->zClient;}

	//Returns player name
	std::string GetPlayerName() const {return this->zUserName;}
	//
	std::string GetModelPath() const {return this->zUserModel;}
	//
	bool IsReady() const {return this->zReady;}

protected:
	virtual void OnEvent( Event* e);

private:
	friend class Game;
};