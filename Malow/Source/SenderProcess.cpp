#include "SenderProcess.h"
#include "SendPacketEvent.h"
#include "ClientChannel.h"

using namespace MaloW;

class QuitProcessMessage : public MaloW::ProcessEvent
{
public:

};

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
			else if ( QuitProcessMessage* QPE = dynamic_cast<QuitProcessMessage*>(PE) )
			{
				delete PE;
				return;
			}
		}
		catch(...)
		{

		}

		delete PE;
	}
}

void SenderProcess::CloseSpecific()
{
	this->PutEvent( new QuitProcessMessage() );
}