#pragma once

#include "Process.h"

namespace MaloW
{
	class NetworkChannel;
	class ClientChannel;
};

namespace MaloW
{
	class SenderProcess : public Process
	{
	private:
		Process* zObserverProcess;
		ClientChannel* zClientChannel;

	protected:
		SenderProcess(Process* observerProcess, ClientChannel* channel);
		virtual ~SenderProcess();

		// Running Process
		virtual void Life();

		friend ClientChannel;
	};
}