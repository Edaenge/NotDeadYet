/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/


#pragma once

#include "Process.h"
#include "ClientChannel.h"

/*
This class is used when a client connects to the server.
This class is not responsible for deallocation of ClientChannel.
*/
class ClientConnectedEvent : public MaloW::ProcessEvent
{
private:
	MaloW::ClientChannel* zChannel;

	ClientConnectedEvent(MaloW::ClientChannel* channel) :
		zChannel(channel)
	{
	}

public:
	inline MaloW::ClientChannel* GetClientChannel() const { return zChannel; }

	friend class ServerListener;
};
