#pragma once

#include "Process.h"
#include "ClientChannel.h"

/*
This class is used when a client connects to the server.
This class is not responsible for deallocation of ClientChannel.
*/
class ClientDisconnectedEvent : public MaloW::ProcessEvent
{
private:
	MaloW::ClientChannel* zChannel;

	ClientDisconnectedEvent(MaloW::ClientChannel* channel) :
		zChannel(channel)
	{
	}

public:
	inline MaloW::ClientChannel* GetClientChannel() const { return zChannel; }

	friend class MaloW::ClientChannel;
};
