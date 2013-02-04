#pragma once

#include "Process.h"
#include "ServerListener.h"
#include "ClientChannel.h"
#include <map>
#include <mutex>

#ifdef _DEBUG
#pragma comment(lib, "MalowD.lib")
#else
#pragma comment(lib, "Malow.lib")
#endif // _DEBUG


class MasterServer : public MaloW::Process
{
	const unsigned short zPort;
	ServerListener zListener;
	std::map< long, MaloW::ClientChannel* > zChannels;
	std::mutex zChannelsMutex;

public:
	MasterServer( const unsigned short &port );

	void HandleConnected( MaloW::ClientChannel *channel );
	void HandleMessage( MaloW::ClientChannel *channel, const std::string& message );
	void HandleDisconnected( MaloW::ClientChannel *channel );

	void Life();
};