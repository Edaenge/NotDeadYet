#include "ServerChannel.h"
#include "NetworkPacket.h"
#include "Winsock.h"
#include "DisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;



ServerChannel::ServerChannel( MaloW::Process* observerProcess, const std::string &IP, const unsigned int &port ) :
	zNotifier(observerProcess)
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

bool ServerChannel::Receive( std::string& msg )
{
	// Error Code
	int errCode;

	// Receive Packet Size
	unsigned int packetSize = 0;
	if ( (errCode = recv(zSocket, reinterpret_cast<char*>(&packetSize), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Receiving Packet Size!", WSAGetLastError()) );
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

	// Receive Packet
	msg.resize(packetSize);
	if ( (errCode = recv(zSocket, &msg[0], packetSize, 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Receiving Packet Data!", WSAGetLastError()) );
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

	return true;
}

bool ServerChannel::Send(const std::string& msg)
{
	// ErrorCode
	int errCode;

	// Send Packet Size
	unsigned int size = msg.length();
	if( (errCode = send(zSocket, reinterpret_cast<char*>(&size), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Sending Packet Size!", WSAGetLastError()) );
		}
		else
		{
			return false;
		}
	}

	// Send Packet Data
	if( (errCode = send(zSocket, &msg[0], msg.length(), 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Sending Packet Data!", WSAGetLastError()) );
		}
		else
		{
			return false;
		}
	}

	return true;
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

bool MaloW::ServerChannel::TrySend( const std::string& msg )
{
	try
	{
		return Send(msg);
	}
	catch(...)
	{
		return false;
	}
}
