/*
Made by Ed�nge Simon 
for project Not Dead Yet at Blekinge tekniska h�gskola.
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
public:
	ClientConnectedEvent(MaloW::ClientChannel* clientChannel);
	virtual ~ClientConnectedEvent();

	MaloW::ClientChannel* GetClientChannel() const;
private:
	MaloW::ClientChannel* ch;

};
