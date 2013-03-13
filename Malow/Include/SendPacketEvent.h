#pragma once

// ClientChannel uses this event to tell SenderProcess to start sending

#include "Process.h"
#include <string>


namespace MaloW
{
	class SendPacketEvent : public ProcessEvent
	{
		std::string zMessage;

	public:
		SendPacketEvent(const std::string& message) : 
			zMessage(message)
		{
	
		}
	
		inline const std::string& GetMessage() const { return zMessage; }
	};
}