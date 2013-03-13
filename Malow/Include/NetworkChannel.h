#pragma once

#include <string>
#include "Winsock.h"
#include <mutex>


namespace MaloW
{
	class NetworkChannel
	{
	protected:
		SOCKET zSocket;
		std::mutex zWriteMutex;
		std::mutex zReadMutex;
		
		// Counters
		unsigned int zPacketNumberIn;
		unsigned int zPacketNumberOut;
		unsigned int zNumBytesIn;
		unsigned int zNumBytesOut;
		
		// Timing
		__int64 zChannelCreated;

	public:
		// Constructor
		NetworkChannel( SOCKET socket );

		// Amount of packets sent
		inline unsigned int GetNumPacketsSent() const { return zPacketNumberOut; }

		// Amount of packets received
		inline unsigned int GetNumPacketsReceived() const { return zPacketNumberIn; }
		
		// Amount of bytes sent
		inline unsigned int GetNumBytesSent() const { return zNumBytesIn; }

		// Amount of bytes received
		inline unsigned int GetNumBytesReceived() const { return zNumBytesOut; }

		// Receive message
		virtual bool Receive(std::string& msg, double& timeTaken) throw(...);

		// Send message
		virtual bool Send(const std::string& msg) throw(...);

		// Try sending a message, does not throw when failing
		virtual bool TrySend(const std::string& msg);

		// Disconnects the channel
		virtual void Disconnect();
	};
}