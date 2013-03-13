#pragma once

#include "Process.h"

namespace MaloW
{
	class NetworkChannel;
	class ClientChannel;
};

namespace MaloW
{
	class SenderProcess : public MaloW::Process
	{
	private:
		MaloW::Process* zObserverProcess;
		MaloW::ClientChannel* zClientChannel;

	protected:
		SenderProcess(MaloW::Process* observerProcess, MaloW::ClientChannel* channel);
		virtual ~SenderProcess();

		// Running Process
		virtual void Life();

		friend ClientChannel;
	};
}