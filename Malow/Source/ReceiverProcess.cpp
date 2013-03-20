#include "ReceiverProcess.h"
#include "ClientDisconnectedEvent.h"
#include "ClientChannel.h"

using namespace MaloW;


ReceiverProcess::ReceiverProcess(MaloW::Process* observerProcess, MaloW::ClientChannel* channel) : 
	zObserverProcess(observerProcess),
	zClientChannel(channel)
{
	
}

ReceiverProcess::~ReceiverProcess()
{

}

void ReceiverProcess::Life()
{
	MaloW::Debug("Receiver Process Started");

	try
	{
		double packetTime;
		std::string msg;

		while(this->stayAlive)
		{
			if ( zClientChannel->GetNetworkChannel() )
			{
				if ( zClientChannel->GetNetworkChannel()->Receive(msg, packetTime) )
				{
					zObserverProcess->PutEvent(new NetworkPacket(msg, zClientChannel->GetNetworkChannel(), packetTime));
				}
				else
				{
					this->Close();
				}
			}
		}
	}
	catch(...)
	{
		if ( zObserverProcess ) zObserverProcess->PutEvent(new ClientDisconnectedEvent(zClientChannel));
	}
}

void ReceiverProcess::CloseSpecific()
{
	zObserverProcess = 0;
}
