#pragma once

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include "NetworkPacket.h"
#include "NetworkChannel.h"

namespace MaloW
{
	class NetworkChannel;
	class ReceiverProcess;
	class SenderProcess;
}

class ClientData;

using namespace std;

/*
Implement your own client class with this class as an object for sending / receiving data.
1. Create ClientChannel Object.
2. Use setNotifier to set which process received data will be sent to.
3. Call on ->Start
This class will be listening for data with its own process and it will send data with
->SendData(string).
*/

namespace MaloW
{
	class ClientChannel
	{
	private:
		SOCKET zSocket;
		MaloW::NetworkChannel* zNetworkChannel;
		MaloW::Process* zObserverProcess;
		std::string zIP;

		// Packet Receiver
		MaloW::ReceiverProcess* zReceiveProcess;

		// Packet Sender
		MaloW::SenderProcess* zSenderProcess;

		virtual ~ClientChannel();
	public:
		ClientChannel(MaloW::Process* zNotifier, SOCKET hClient, const std::string& ip_adress);
		

		// Get Network Channel
		inline MaloW::NetworkChannel* GetNetworkChannel() const { return zNetworkChannel; }

		// Begin Process
		void Start();

		// Disconnect
		void Disconnect();

		// Send Message
		void Send(const std::string& message);

		// Try Sending
		void TrySend(const std::string& message);

		// The IP Of Connection
		inline const std::string& GetIP() const { return this->zIP; }

		friend class ClientData;
	};
}