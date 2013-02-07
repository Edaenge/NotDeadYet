#pragma once

#include <string>
#include "Winsock.h"

namespace MaloW
{
	class NetworkChannel
	{
	protected:
		SOCKET zSocket;

		NetworkChannel( SOCKET socket );

		// Receive Message
		virtual bool Receive(std::string& msg) throw(...);

	public:
		// Send Message
		virtual bool Send(const std::string& msg) throw(...);

		// Try Sending A message, does not throw when failing
		virtual bool TrySend(const std::string& msg);

		// Disconnects the channel.
		virtual void Disconnect();
	};
}