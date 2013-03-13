#pragma once

#include "Process.h"
#include "ClientChannel.h"


class ClientDroppedEvent : public MaloW::ProcessEvent
{
private:
	MaloW::ClientChannel* zChannel;

	ClientDroppedEvent(MaloW::ClientChannel* channel) :
		zChannel(channel)
	{
	}

public:
	inline MaloW::ClientChannel* GetClientChannel() const { return zChannel; }

	friend class MaloW::ClientChannel;
};
