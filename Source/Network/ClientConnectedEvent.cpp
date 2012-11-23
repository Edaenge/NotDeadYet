#include "ClientConnectedEvent.h"


ClientConnectedEvent::ClientConnectedEvent(MaloW::ClientChannel* clientChannel)
{
	this->ch = clientChannel;
}

ClientConnectedEvent::~ClientConnectedEvent()
{
}

MaloW::ClientChannel* ClientConnectedEvent::GetClientChannel() const
{
	return this->ch;
}
