#pragma once

#include "Process.h"

namespace MaloW
{
	class ClientChannel;
	class ReceiverProcess;
}

/*
This class is used when a client disconnects from the server.
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
	friend class MaloW::ReceiverProcess;
};
