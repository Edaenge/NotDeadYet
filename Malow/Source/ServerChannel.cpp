#include "ServerChannel.h"
#include "NetworkPacket.h"
#include "Winsock.h"
#include "DisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;


ServerChannel::ServerChannel( MaloW::Process* observerProcess, const std::string &IP, const unsigned int &port,
							 std::string& out, int& errorCode)
	: zNotifier(observerProcess), NetworkChannel(0)
{
	try
	{
		WSADATA wsaData;
		if(int errCode = WSAStartup(MAKEWORD(2,2), &wsaData)) 
		{
			out = "Failed To Initializing network connection!";
			errorCode = WSAGetLastError();
			throw( NetworkException("Failed Initializing Winsock!", WSAGetLastError()) );
		}
	}
	catch (NetworkException* e)
	{
		out = e->errString;
		errorCode = e->errCode;
	}

	try
	{
		// Open a socket
		zSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if(!zSocket) 
		{
			out = "Failed Opening Socket!";
			errorCode = WSAGetLastError();
			return;
			//throw( NetworkException("Failed Opening Socket!", WSAGetLastError()) );
		}
	}
	catch (NetworkException* e)
	{
		out = e->errString;
		errorCode = e->errCode;
	}
	
	// Connect
	sockaddr_in saServer;
	saServer.sin_port = htons((u_short)port);
	saServer.sin_addr.s_addr = inet_addr(IP.c_str());
	saServer.sin_family = AF_INET;
	try
	{
		if( connect(zSocket, (sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR )
		{
			closesocket(zSocket);
			out = "Failed Connecting To Server! Verify that the server is online";
			errorCode = WSAGetLastError();
			return;
			//throw( NetworkException("Failed Connecting Socket!", WSAGetLastError()) );
		}
	}
	catch (NetworkException* e)
	{
		out = e->errString;
		errorCode = e->errCode;
	}
	
}

ServerChannel::~ServerChannel()
{
	this->Close();
	this->WaitUntillDone();
	if(zSocket) 
		closesocket(zSocket);
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
		try
		{
			if( shutdown(zSocket, SD_BOTH) == SOCKET_ERROR )
			{
				throw( NetworkException("Failed Shutting Down Socket!", WSAGetLastError()) );
			}
		}
		catch (...)
		{
			
		}
		
	}
}
