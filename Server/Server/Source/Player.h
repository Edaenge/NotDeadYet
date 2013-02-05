#pragma once

#include "ClientChannel.h"

class Player
{
	MaloW::ClientChannel* zChannel;
public:
	Player( MaloW::ClientChannel* channel );
	virtual ~Player();

	// Disconnect This Player
	void Kick();
};