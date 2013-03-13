#ifndef SERVERCHANNEL_H
#define SERVERCHANNEL_H

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "Process.h"
#include "NetworkChannel.h"

/*
Implement your own client class with this class as an object for sending / recieving data.
1. Create ServerChannel Object.
2. Use setNotifier to set which process recieved data will be sent to.
3. Call on ->Start
This class will create a channel to a server that has a process that will read data from the
server and you can also send data to the server.s
*/

namespace MaloW
{
	class ServerChannel : public MaloW::NetworkChannel, public MaloW::Process
	{
	private:
		Process* zNotifier;

	protected:
		void CloseSpecific();

	public:
		ServerChannel(MaloW::Process* observerProcess);
		virtual ~ServerChannel();

		// Begin Process
		void Life();
		bool Connect(const std::string &IP, const unsigned int &port);
	};
}

#endif