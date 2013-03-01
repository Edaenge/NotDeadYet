#include "ServerListener.h"
#include "ClientConnectedEvent.h"
#include "NetworkException.h"


ServerListener::ServerListener( MaloW::Process *observer, const unsigned int &port ) : 
	zObserver(observer), 
	zPort(port)
{
	WSADATA wsaData;
	if ( WSAStartup(MAKEWORD(2,2), &wsaData) )
	{
		throw NetworkException("Failed WSA Startup!", WSAGetLastError());
	}

	if ((zListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET) 
	{
		throw NetworkException("Failed Creating Listen Socket!", WSAGetLastError());
	}

	sockaddr_in saListen;
	saListen.sin_port = htons((u_short)port);
	saListen.sin_addr.s_addr = htonl(INADDR_ANY);
	saListen.sin_family = AF_INET;
	if ( bind(zListenSocket, (sockaddr*)&saListen, sizeof(sockaddr)) == SOCKET_ERROR  )
	{
		throw NetworkException("Failed Binding Listen Socket!", WSAGetLastError());
	}

	if ( listen(zListenSocket, SOMAXCONN) == SOCKET_ERROR )
	{
		throw NetworkException("Failed Initializing Listen Socket!", WSAGetLastError());
	}
}

ServerListener::~ServerListener()
{
	if ( zListenSocket ) closesocket(zListenSocket);

	this->Close();
	this->WaitUntillDone();
}

bool ServerListener::Accept( SOCKET &newConnection, sockaddr_in &client )
{
	int nSALen = sizeof( sockaddr );
	newConnection = accept(zListenSocket, (sockaddr*)&client, &nSALen);
	
	if ( newConnection == SOCKET_ERROR )
	{
		if ( newConnection < 0 )
		{
			throw NetworkException("Failed Accepting On Listen Socket!", WSAGetLastError());
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
	sockaddr_in client = { 0 };

	while(this->stayAlive)
	{
		this->stayAlive = Accept(newConnection, client);
		char *connected_ip = inet_ntoa(client.sin_addr);
		std::string ip(connected_ip);

		zObserver->PutEvent(new ClientConnectedEvent(new MaloW::ClientChannel(zObserver, newConnection, ip)));
	}
}