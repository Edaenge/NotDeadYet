#ifndef SERVERCHANNEL_H
#define SERVERCHANNEL_H

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "Process.h"

using namespace std;

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
	class ServerChannel : public MaloW::Process
	{
	private:
		SOCKET zSocket;
		Process* zNotifier;

		// Receive Message
		bool Receive(std::string& msg) throw(...);

	protected:
		void CloseSpecific();

	public:
		ServerChannel( MaloW::Process* observerProcess, const std::string &IP, const unsigned int &port );
		virtual ~ServerChannel();

		// Begin Process
		void Life();

		// Send Data Through Channel
		bool Send(const std::string& msg) throw(...);

		// Send Data, Does not throw anything back
		bool TrySend(const std::string& msg);
	};
}

#endif