#include "ServerChannel.h"
#include "NetworkPacket.h"
#include "Winsock.h"
#include "DisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;


ServerChannel::ServerChannel( MaloW::Process* observerProcess, const std::string &IP, const unsigned int &port ) :
	zNotifier(observerProcess), 
	NetworkChannel(0)
{
	WSADATA wsaData;
	if(int errCode = WSAStartup(MAKEWORD(2,2), &wsaData)) 
	{
		throw( NetworkException("Failed Initializing Winsock!", WSAGetLastError()) );
	}

	// Open a socket
	zSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(!zSocket) 
	{
		throw( NetworkException("Failed Opening Socket!", WSAGetLastError()) );
	}

	// Connect
	sockaddr_in saServer;
	saServer.sin_port = htons((u_short)port);
	saServer.sin_addr.s_addr = inet_addr(IP.c_str());
	saServer.sin_family = AF_INET;
	if( connect(zSocket, (sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR )
	{
		closesocket(zSocket);
		throw( NetworkException("Failed Connecting Socket!", WSAGetLastError()) );
	}
}

ServerChannel::~ServerChannel()
{
	this->Close();
	this->WaitUntillDone();
	if(zSocket) closesocket(zSocket);
}

void ServerChannel::Life()
{
	MaloW::Debug("ServerChannel Process Started");

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
		zNotifier->PutEvent(new DisconnectedEvent(this));
	}
}

void ServerChannel::CloseSpecific()
{
	if(zSocket)
	{
		if( shutdown(zSocket, SD_BOTH) == SOCKET_ERROR )
		{
			throw( NetworkException("Failed Shutting Down Socket!", WSAGetLastError()) );
		}
	}
}
