#ifndef CLIENTCHANNEL_H
#define CLIENTCHANNEL_H

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include "NetworkPacket.h"
#include "NetworkChannel.h"

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
	class ClientChannel : public MaloW::NetworkChannel, public MaloW::Process
	{
	private:
		MaloW::Process* zNotifier;
		long zID;

		// Receive Message
		//bool Receive(std::string& msg) {NetworkChannel::Receive(msg);} throw(...)

	protected:
		void CloseSpecific();

	public:
		ClientChannel(MaloW::Process* zNotifier, SOCKET hClient);
		virtual ~ClientChannel();

		// Begin Process
		void Life();
	};
}


#endif