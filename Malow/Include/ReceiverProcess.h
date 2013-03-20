#pragma once

#include "Process.h"
#include "NetworkChannel.h"

namespace MaloW
{
	class ClientChannel;
};

namespace MaloW
{
	class ReceiverProcess : public MaloW::Process
	{
	private:
		MaloW::Process* zObserverProcess;
		MaloW::ClientChannel* zClientChannel;

	protected:
		ReceiverProcess(MaloW::Process* observerProcess, MaloW::ClientChannel* channel);
		virtual ~ReceiverProcess();

		// Running Process
		virtual void Life();
		
		// Thread has been asked to close
		virtual void CloseSpecific();

		friend ClientChannel;
	};
}