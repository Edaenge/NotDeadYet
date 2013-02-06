#pragma once

#include "Process.h"
#include "NetworkChannel.h"


namespace MaloW
{
	class NetworkPacket : public MaloW::ProcessEvent
	{
	private:
		std::string zMessage;
		MaloW::NetworkChannel* zChannel;

	public:
		NetworkPacket(const std::string& message, MaloW::NetworkChannel* channel) : 
			zMessage(message),
			zChannel(channel)
		{
		}

		const std::string& GetMessage() const { return zMessage; }
		MaloW::NetworkChannel* GetChannel() const { return zChannel; }
	};
}