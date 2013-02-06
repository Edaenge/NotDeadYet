#include "ClientChannel.h"
#include "Winsock.h"
#include "ClientDisconnectedEvent.h"
#include "NetworkException.h"

using namespace MaloW;


ClientChannel::ClientChannel(MaloW::Process* observerProcess, SOCKET sock) :
	zNotifier(observerProcess),
	zSocket(sock)
{
	static unsigned int id = 0;
	zID = id++;
}

ClientChannel::~ClientChannel()
{
	this->Close();
	this->WaitUntillDone();

	if(zSocket) closesocket(zSocket);
}

bool ClientChannel::Receive( std::string& msg )
{
	// Receive Packet Size
	unsigned int packetSize = 0;
	int retCode;
	if ( (retCode = recv(zSocket, reinterpret_cast<char*>(&packetSize), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( retCode < 0 )
		{
			throw NetworkException("Failed Receiving Packet Size!", WSAGetLastError());
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

	// Receive Packet
	msg.resize(packetSize);
	if ( (retCode = recv(zSocket, &msg[0], packetSize, 0)) <= 0 )
	{
		if ( retCode < 0 )
		{
			throw NetworkException("Failed Receiving Packet Size!", WSAGetLastError());
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

	return true;
}

bool ClientChannel::Send(const std::string& msg)
{
	// Send Packet Size
	int retCode;
	unsigned int size = msg.length();
	if( (retCode = send(zSocket, reinterpret_cast<char*>(&size), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( retCode < 0 )
		{
			throw NetworkException("Failed Sending Packet Size!", WSAGetLastError());
		}
		else
		{
			return false;
		}
	}

	// Send Packet Data
	if( (retCode = send(zSocket, &msg[0], msg.length(), 0)) <= 0 )
	{
		if ( retCode < 0 )
		{
			throw NetworkException("Failed Sending Packet Data!", WSAGetLastError());
		}
		else
		{
			return false;
		}
	}

	return true;
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
	CloseSpecific();
	this->zNotifier->PutEvent(new ClientDisconnectedEvent(this));
}

bool MaloW::ClientChannel::TrySend( const std::string& msg )
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
