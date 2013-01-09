//File: C++CLIWrapper.cpp

#include "C++CLIWrapper.h"
#include "../../../Source/MaloWLib/Safe.h"
#include <sstream>

namespace System { namespace Windows { namespace Interop
{
	CppCLI::CppCLI()
	{
		zServer = new ServerEngineWrapper();
	}

	CppCLI::~CppCLI()
	{
		if(zServer)
			delete zServer;
	}

	String^ CppCLI::Init(int port, int nrOfPlayers)
	{
		String^ returnText;
		
		returnText = gcnew String(zServer->Init(port, nrOfPlayers));

		return returnText;
	}

	int CppCLI::GetNrOfPlayers()
	{
		return zServer->GetNrOfPlayers();
	}

	String^ CppCLI::ProcessText(String^ msg)
	{
		char* lpText = nullptr;
		String^ returnText;

		//Konvertera String^ -> char*
		try
		{
			lpText = (char*)Marshal::StringToHGlobalAnsi(msg).ToPointer();

			returnText = gcnew String(zServer->ProcessText(lpText));
		}
		finally
		{
			Marshal::FreeHGlobal((IntPtr) const_cast<char*>(lpText)); // Free memory
		}

		return returnText;
	}

	bool CppCLI::StartHost()
	{
		return zServer->StartHost();
	}

	void CppCLI::ShutdownHost()
	{
		return zServer->ShutdownHost();
	}

	bool CppCLI::IsRunning()
	{
		return zServer->IsRunning();
	}



}}}