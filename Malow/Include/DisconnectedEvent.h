#pragma once

#include "ServerChannel.h"
#include "MaloW.h"


class DisconnectedEvent : public MaloW::ProcessEvent
{
	MaloW::ServerChannel *zChannel;
	std::string zReason;
	DisconnectedEvent( MaloW::ServerChannel* channel, std::string reason) :
		zChannel(channel), zReason(reason)
	{
	}

public:
	virtual ~DisconnectedEvent() {};

	std::string GetReason() {return this->zReason;}
	inline MaloW::ServerChannel* GetChannel() const { return this->zChannel; }

	friend class MaloW::ServerChannel;
};