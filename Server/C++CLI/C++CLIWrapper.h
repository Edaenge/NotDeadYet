//File C++CLIWrapper.h


#pragma once

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include <ServerEngineWrapper.h>

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows;
using namespace System::Runtime::InteropServices;

namespace System { namespace Windows { namespace Interop
{
	public ref class CppCLI
	{
	protected:
		ServerEngineWrapper* zServer;
	public:
		CppCLI();
		~CppCLI();

		String^ Init(int port, int nrOfPlayers, String^ gameMode, String^ mapName);
		void ShutdownHost();
		bool StartHost();

		bool IsRunning();
		int GetNrOfPlayers();
	};
}}}