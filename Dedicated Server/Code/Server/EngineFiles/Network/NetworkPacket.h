#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include "../../../../../Source/MaloWLib/Process.h"

namespace MaloW
{
	class NetworkPacket : public MaloW::ProcessEvent
	{
	private:
		string message;
		int id;

	public:
		NetworkPacket(string message, int SenderID) { this->message = message; this->id = SenderID; }
		virtual ~NetworkPacket() { }
		string getMessage() { return this->message; }
		int getID() { return this->id; }
	};
}

#endif