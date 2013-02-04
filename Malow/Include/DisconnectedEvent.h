#pragma once

#include "ServerChannel.h"
#include "MaloW.h"


class DisconnectedEvent : public MaloW::ProcessEvent
{
	MaloW::ServerChannel *zChannel;
	DisconnectedEvent( MaloW::ServerChannel* channel ) :
		zChannel(channel)
	{
	}

public:
	virtual ~DisconnectedEvent() {};

	inline MaloW::ServerChannel* GetChannel() const { return zChannel; }

	friend class MaloW::ServerChannel;
};