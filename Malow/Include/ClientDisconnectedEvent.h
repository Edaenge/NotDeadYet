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
namespace MaloW
{
	class ClientDisconnectedEvent : public ProcessEvent
	{
	private:
		ClientChannel* zChannel;
	
		ClientDisconnectedEvent(ClientChannel* channel) :
			zChannel(channel)
		{
		}
	
	public:
		inline ClientChannel* GetClientChannel() const { return zChannel; }
	
		friend class ClientChannel;
		friend class ReceiverProcess;
	};
}