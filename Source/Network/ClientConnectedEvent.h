/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/


#pragma once

#include "Process.h"
#include "Network/ClientChannel.h"

/*
This class is used when a client connects to the server.
This class is not responsible for deallocation of ClientChannel.
*/
class ClientConnectedEvent : public MaloW::ProcessEvent
{
public:
	ClientConnectedEvent(MaloW::ClientChannel* clientChannel);
	virtual ~ClientConnectedEvent();

	MaloW::ClientChannel* GetClientChannel() const;
private:
	MaloW::ClientChannel* ch;

};
