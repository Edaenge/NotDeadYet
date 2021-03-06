#include "ServerChannel.h"
#include "NetworkPacket.h"
#include "Winsock.h"
#include "DisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;


ServerChannel::ServerChannel( MaloW::Process* observerProcess)
	: zNotifier(observerProcess), NetworkChannel(0)
{
}

ServerChannel::~ServerChannel()
{
	if (zSocket)
		shutdown(zSocket, SD_BOTH);

	this->Close();
	this->WaitUntillDone();
	
	if(zSocket)
		closesocket(zSocket);
}

bool MaloW::ServerChannel::Connect( const std::string &IP, const unsigned int &port )
{
	// Start WSA
	WSADATA wsaData;
	if(int errCode = WSAStartup(MAKEWORD(2,2), &wsaData)) 
	{
		throw NetworkException("Failed To Initializing Socket!", WSAGetLastError());
		return false;
	}

	// Open a socket
	zSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(!zSocket) 
	{
		throw NetworkException("Failed Opening Socket!", WSAGetLastError());
		return false;
	}

	// Connect info
	sockaddr_in saServer;
	saServer.sin_port = htons((u_short)port);
	saServer.sin_addr.s_addr = inet_addr(IP.c_str());
	saServer.sin_family = AF_INET;

	int timeOut = 10000;
	setsockopt(zSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeOut, sizeof(int));

	// Attempt connection
	if( connect(zSocket, (sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();
		closesocket(zSocket);
		throw NetworkException("Failed Connecting to Server!", errorCode);
		return false;
	}

	return true;
}

void ServerChannel::Life()
{
	MaloW::Debug("ServerChannel Process Started");

	try
	{
		double packetTime;
		std::string msg;
		while(this->stayAlive && Receive(msg, packetTime))
		{
			zNotifier->PutEvent(new NetworkPacket(msg, this, packetTime));
		}
	}
	catch(char* e)
	{
		zNotifier->PutEvent(new DisconnectedEvent(this, e));
	}
	catch(NetworkException e)
	{
		std::string errMsg = "Error Code: " + MaloW::convertNrToString((float)e.errCode) +  e.errString;
		zNotifier->PutEvent(new DisconnectedEvent(this, errMsg));
	}
	catch(...)
	{
		zNotifier->PutEvent(new DisconnectedEvent(this, "Unknown Reason"));
	}
}