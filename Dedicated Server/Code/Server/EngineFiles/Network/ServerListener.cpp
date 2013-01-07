#include "ServerListener.h"


ServerListener::ServerListener() : NetworkServer()
{

}

ServerListener::~ServerListener()
{

}

void ServerListener::ClientConnected(MaloW::ClientChannel* cc)
{
	ClientConnectedEvent* cce = new ClientConnectedEvent(cc);
	this->PutEvent(cce);
}

int ServerListener::InitListener(int port)
{
	return this->InitConnection(port);
}
