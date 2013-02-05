#pragma once

#include <Observer.h>
#include <map>
#include <KeyStates.h>

class ClientData;
class World;
class GameMode;
class Player;
class Behavior;

class Game : public Observer, public Observed
{
	GameMode* zGameMode;
	World* zWorld;
	std::map<ClientData*, Player*> _players;
	std::map<Player*, Behavior*> _behaviors;
	std::map<Player*, KeyStates> _keyStates;

public:
	Game( GameMode* mode, World* world );
	virtual ~Game();

	// Returns false if game has finished
	bool Update( float dt );

	// Event input
	void OnEvent( Event* e );
};