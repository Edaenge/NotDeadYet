#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include "Process.h"
#include "ClientChannel.h"

namespace MaloW
{
	class NetworkPacket : public MaloW::ProcessEvent
	{
	private:
		std::string message;
		ClientChannel* cc;

	public:
		NetworkPacket(std::string message, ClientChannel* sender) { this->message = message; this->cc = sender; }
		virtual ~NetworkPacket() { }
		std::string getMessage() { return this->message; }
		ClientChannel* getChannel() { return this->cc; }
	};
}

#endif