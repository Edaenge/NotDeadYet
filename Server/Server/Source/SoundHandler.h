#pragma once
#include "Observer.h"
#include "Player.h"

class SoundHandler : public Observer
{
private:
	std::set<Player*> zPlayers;
public:
	SoundHandler();
	virtual ~SoundHandler();

protected:
	void OnEvent(Event* e);
};