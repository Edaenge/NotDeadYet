#include "ServerListener.h"
#include "ClientConnectedEvent.h"



ServerListener::ServerListener( MaloW::Process *observer, const unsigned int &port ) : 
	zObserver(observer), 
	zPort(port)
{

	WSADATA wsaData;
	if ( WSAStartup(MAKEWORD(2,2), &wsaData) )
	{
		throw("Failed WSA Startup!");
	}

	if ((zListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET) 
	{
		throw("Failed Creating Listen Socket!");
	}

	sockaddr_in saListen;
	saListen.sin_port = htons((u_short)port);
	saListen.sin_addr.s_addr = htonl(INADDR_ANY);
	saListen.sin_family = AF_INET;
	if ( bind(zListenSocket, (sockaddr*)&saListen, sizeof(sockaddr)) == SOCKET_ERROR  )
	{
		throw("Failed Binding Listen Socket!");
	}

	if ( listen(zListenSocket, SOMAXCONN) == SOCKET_ERROR )
	{
		throw("Failed Initializing Listen Socket!");
	}
}

ServerListener::~ServerListener()
{
	this->Close();
	this->WaitUntillDone();
	if ( zListenSocket ) closesocket(zListenSocket);
}

bool ServerListener::Accept( SOCKET &newConnection )
{
	sockaddr_in saClient = { 0 };
	int nSALen = sizeof( sockaddr );
	newConnection = accept(zListenSocket, (sockaddr*)&saClient, &nSALen);
	
	if ( newConnection == SOCKET_ERROR )
	{
		if ( newConnection < 0 )
		{
			throw("Failed Accepting On Listen Socket!");
		}
		else
		{
			return false;
		}
	}

	return true;
}

void ServerListener::Life()
{
	MaloW::Debug("ClientChannel Listener Started");

	SOCKET newConnection;
	while(this->stayAlive && Accept(newConnection))
	{
		zObserver->PutEvent(new ClientConnectedEvent(new MaloW::ClientChannel(zObserver, newConnection)));
	}
}