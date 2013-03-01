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
		double zTimeTaken;

	public:
		NetworkPacket(const std::string& message, MaloW::NetworkChannel* channel, const double& timeTaken) : 
			zMessage(message),
			zChannel(channel),
			zTimeTaken(timeTaken)
		{
		}

		const double& GetTimeTaken() const { zTimeTaken; }
		const std::string& GetMessage() const { return zMessage; }
		MaloW::NetworkChannel* GetChannel() const { return zChannel; }
	};
}