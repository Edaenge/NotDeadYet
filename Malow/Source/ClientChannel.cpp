#include "ClientChannel.h"
#include "Winsock.h"
#include "ClientDisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;


ClientChannel::ClientChannel(MaloW::Process* observerProcess, SOCKET sock) :
	zNotifier(observerProcess),
	NetworkChannel(sock)
{
	static unsigned int id = 0;
	this->zID = id++;
}

ClientChannel::~ClientChannel()
{
	this->Close();
	this->WaitUntillDone();

	if(zSocket) closesocket(zSocket);
}

void ClientChannel::Life()
{
	MaloW::Debug("ClientChannel Process Started");

	try
	{
		std::string msg;
		while(this->stayAlive && Receive(msg))
		{
			zNotifier->PutEvent(new NetworkPacket(msg, this));
		}
	}
	catch(...)
	{
		zNotifier->PutEvent(new ClientDisconnectedEvent(this));
	}
}

void ClientChannel::CloseSpecific()
{
	if(zSocket)
	{
		if( shutdown(zSocket, SD_BOTH) == SOCKET_ERROR )
		{
			throw NetworkException("Failed Shutting Down Socket!", WSAGetLastError());
		}
	}
}

void MaloW::ClientChannel::Disconnect()
{
	NetworkChannel::Disconnect();
	
	this->zNotifier->PutEvent(new ClientDisconnectedEvent(this));
}
