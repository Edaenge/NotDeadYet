#pragma once

/*
Made by Ed�nge Simon 
for project Not Dead Yet at Blekinge tekniska h�gskola.

This class is used when a client connects to the server.
This class is not responsible for deallocation of ClientChannel.
*/

#include "Process.h"
#include "ClientChannel.h"


namespace MaloW
{
	class ClientConnectedEvent : public ProcessEvent
	{
	private:
		ClientChannel* zChannel;
	
		ClientConnectedEvent(ClientChannel* channel) :
			zChannel(channel)
		{
		}
	
	public:
		inline ClientChannel* GetClientChannel() const { return zChannel; }
	
		friend class ServerListener;
	};
}