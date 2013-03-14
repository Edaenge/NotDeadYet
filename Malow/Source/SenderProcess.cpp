#include "SenderProcess.h"
#include "SendPacketEvent.h"
#include "ClientChannel.h"

using namespace MaloW;


SenderProcess::SenderProcess(MaloW::Process* observerProcess, MaloW::ClientChannel* channel) :
	zObserverProcess(observerProcess),
	zClientChannel(channel)
{

}

SenderProcess::~SenderProcess()
{

}

void SenderProcess::Life()
{
	while ( ProcessEvent* PE = this->WaitEvent() )
	{
		try
		{
			if ( SendPacketEvent* SPE = dynamic_cast<SendPacketEvent*>(PE) )
			{
				if ( zClientChannel->GetNetworkChannel() )
				{
					zClientChannel->GetNetworkChannel()->Send(SPE->GetMessage());
				}
			}
		}
		catch(...)
		{

		}

		delete PE;
	}
}