#include "ServerListener.h"
#include "ClientConnectedEvent.h"



ServerListener::ServerListener( MaloW::Process *observer, const unsigned int &port ) : 
	zObserver(observer), 
	zPort(port)
{

	WSADATA wsaData;
	if ( WSAStartup(MAKEWORD(2,2), &wsaData) )
	{
		std::stringstream ss;
		ss << "Failed WSA Startup! ErrCode: " << WSAGetLastError();
		throw(ss.str().c_str());
	}

	if ((zListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET) 
	{
		std::stringstream ss;
		ss << "Failed Creating Listen Socket! ErrCode: " << WSAGetLastError();
		throw(ss.str().c_str());
	}

	sockaddr_in saListen;
	saListen.sin_port = htons((u_short)port);
	saListen.sin_addr.s_addr = htonl(INADDR_ANY);
	saListen.sin_family = AF_INET;
	if ( bind(zListenSocket, (sockaddr*)&saListen, sizeof(sockaddr)) == SOCKET_ERROR  )
	{
		std::stringstream ss;
		ss << "Failed Binding Listen Socket! ErrCode: " << WSAGetLastError();
		throw(ss.str().c_str());
	}

	if ( listen(zListenSocket, SOMAXCONN) == SOCKET_ERROR )
	{
		std::stringstream ss;
		ss << "Failed Initializing Listen Socket! ErrCode: " << WSAGetLastError();
		throw(ss.str().c_str());
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
			std::stringstream ss;
			ss << "Failed Accepting On Listen Socket! ErrCode: " << WSAGetLastError();
			throw(ss.str().c_str());
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