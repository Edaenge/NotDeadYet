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
		NetworkPacket(string message, MaloW::NetworkChannel* channel) : 
			zMessage(message),
			zChannel(channel)
		{
		}

		virtual ~NetworkPacket() {}
		const std::string& GetMessage() const { return zMessage; }
		MaloW::NetworkChannel* GetChannel() const { return zChannel; }
	};
}