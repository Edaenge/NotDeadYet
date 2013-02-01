#include "ServerChannel.h"
#include "NetworkPacket.h"
#include "Winsock.h"
#include "NetworkMessageConverter.h"
#include "../GameFiles/ClientServerMessages.h"
#include "DisconnectedEvent.h"

using namespace MaloW;



ServerChannel::ServerChannel( MaloW::Process* observerProcess, const std::string &IP, const unsigned int &port ) :
	zNotifier(observerProcess)
{
	WSADATA wsaData;
	if(int errCode = WSAStartup(MAKEWORD(2,2), &wsaData)) 
	{
		std::stringstream ss;
		ss << "Failed Initializing Winsock! ErrCode: " << WSAGetLastError();
		throw(ss.str());
	}

	// Open a socket
	zSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(!zSocket) 
	{
		std::stringstream ss;
		ss << "Failed Opening Socket! ErrCode:" << WSAGetLastError();
		throw(ss.str());
	}

	// Connect
	sockaddr_in saServer;
	saServer.sin_port = htons((u_short)port);
	saServer.sin_addr.s_addr = inet_addr(IP.c_str());
	saServer.sin_family = AF_INET;
	if( connect(zSocket, (sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR )
	{
		closesocket(zSocket);
		std::stringstream ss;
		ss << "Failed Connecting Socket! ErrCode:" << WSAGetLastError();
		throw(ss.str());
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
			std::stringstream ss;
			ss << "Failed Receiving Packet Size! ErrCode:" << WSAGetLastError();
			throw(ss.str());
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
			std::stringstream ss;
			ss << "Failed Receiving Packet Size! ErrCode:" << WSAGetLastError();
			throw(ss.str());
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
			std::stringstream ss;
			ss << "Failed Sending Packet Size! ErrCode:" << WSAGetLastError();
			throw(ss.str());
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
			std::stringstream ss;
			ss << "Failed Sending Packet Data! ErrCode:" << WSAGetLastError();
			throw(ss.str());
		}
		else
		{
			return false;
		}
	}

	// Log Messages Sent
	if (Messages::MsgFileWrite())
		Messages::Debug("Sc: Sending to Server " + msg);

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
			zNotifier->PutEvent(new NetworkPacket(msg, this->getID()));
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
			std::stringstream ss;
			ss << "Failed Shutting Down Socket! ErrCode:" << WSAGetLastError();
			throw(ss.str());
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
