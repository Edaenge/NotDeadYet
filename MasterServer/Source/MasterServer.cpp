#include "MasterServer.h"
#include "ClientConnectedEvent.h"
#include "ClientDisconnectedEvent.h"


MasterServer::MasterServer( const unsigned short &port ) :
	zPort(port),
	zListener(this, port)
{
	zListener.Start();
}

void MasterServer::HandleConnected( MaloW::ClientChannel *channel )
{
	zChannelsMutex.lock();
	zChannels[channel->GetClientID()] = channel;
	zChannelsMutex.unlock();

	channel->Start();
}

void MasterServer::HandleMessage( MaloW::ClientChannel *channel, const std::string& message )
{

}

void MasterServer::HandleDisconnected( MaloW::ClientChannel *channel )
{
	zChannelsMutex.lock();
	auto i = zChannels.find(channel->GetClientID());
	if ( i != zChannels.end() )
		zChannels.erase(i);
	zChannelsMutex.unlock();

	delete channel;
}

void MasterServer::Life()
{
	while( this->stayAlive )
	{
		MaloW::ProcessEvent* e = this->WaitEvent();

		if (MaloW::NetworkPacket *packetEvent = dynamic_cast<MaloW::NetworkPacket*>(e))
		{
			MaloW::ClientChannel *channel;
			zChannelsMutex.lock();
			channel = zChannels[packetEvent->getID()];
			zChannelsMutex.unlock();
			HandleMessage(channel, packetEvent->getMessage());
		}
		else if (ClientConnectedEvent *connectedEvent = dynamic_cast<ClientConnectedEvent*>(e))
		{
			HandleConnected(connectedEvent->GetClientChannel());
		}
		else if (ClientDisconnectedEvent *DCEvent = dynamic_cast<ClientDisconnectedEvent*>(e))
		{
			HandleDisconnected(DCEvent->GetClientChannel());
		}

		delete e;
	}

	zListener.Close();
	zListener.WaitUntillDone();
}