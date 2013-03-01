#include "NetworkChannel.h"
#include "NetworkException.h"
#include "ClientDisconnectedEvent.h"
#include "Winsock.h"

using MaloW::NetworkChannel;

NetworkChannel::NetworkChannel( SOCKET socket ) : 
	zSocket(socket),
	zPacketNumberIn(0),
	zPacketNumberOut(0),
	zNumBytesIn(0),
	zNumBytesOut(0)
{

}

bool NetworkChannel::Receive( std::string& msg )
{
	// Error Code
	int errCode;

	// Receive Packet Number
	unsigned int packetNumber = 0;
	if ( (errCode = recv(zSocket, reinterpret_cast<char*>(&packetNumber), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Receiving Packet Number!", WSAGetLastError()) );
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

	// Check Packet Number
	if ( packetNumber != zPacketNumberIn++ )
	{
		throw( NetworkException("Packet Not Received In Correct Order!", 0) );
	}

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

	// Count Bytes Received
	zNumBytesIn += packetSize + sizeof(unsigned int) * 2;

	return true;
}

bool NetworkChannel::Send(const std::string& msg)
{
	// ErrorCode
	int errCode;

	// Send Packet Number
	unsigned int packetNumber = zPacketNumberOut++;
	if ( (errCode = send(zSocket, reinterpret_cast<char*>(&packetNumber), sizeof(unsigned int), 0)) <= 0 )
	{
		if ( errCode < 0 )
		{
			throw( NetworkException("Failed Sending Packet Number!", WSAGetLastError()) );
		}
		else
		{
			// Connection Canceled
			return false;
		}
	}

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

	// Count Bytes Sent
	zNumBytesOut += size + sizeof(unsigned int) * 2;

	return true;
}

bool NetworkChannel::TrySend(const std::string& msg)
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

void NetworkChannel::Disconnect()
{
	shutdown(zSocket, SD_BOTH);
}