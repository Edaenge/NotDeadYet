#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include "Process.h"

namespace MaloW
{
	class NetworkPacket : public MaloW::ProcessEvent
	{
	private:
		string message;
		MaloW::ClientChannel* cc;

	public:
		NetworkPacket(string message, MaloW::ClientChannel* sender) { this->message = message; this->cc = sender; }
		virtual ~NetworkPacket() { }
		string getMessage() { return this->message; }
		MaloW::ClientChannel* getChannel() { return this->cc; }
	};
}

#endif