#ifndef CLIENTCHANNEL_H
#define CLIENTCHANNEL_H

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include "NetworkPacket.h"

using namespace std;

/*
Implement your own client class with this class as an object for sending / recieving data.
1. Create ClientChannel Object.
2. Use setNotifier to set which process recivied data will be sent to.
3. Call on ->Start
This class will be listening for data with its own process and it will send data with
->SendData(string).
*/

namespace MaloW
{
	class ClientChannel : public MaloW::Process
	{
	private:
		SOCKET zSocket;
		MaloW::Process* zNotifier;
		long zID;

		// Receive Message
		bool Receive(std::string& msg) throw(...);

	protected:
		void CloseSpecific();

	public:
		ClientChannel(MaloW::Process* zNotifier, SOCKET hClient);
		virtual ~ClientChannel();

		// Begin Process
		void Life();

		// Send Message
		bool Send(const std::string& msg) throw(...);

		// Connection ID
		inline long GetClientID() const { return zID; }

		// Disconnects the channel. Will push a Player DisconnectedEvent.
		void Disconnect();
	};
}


#endif