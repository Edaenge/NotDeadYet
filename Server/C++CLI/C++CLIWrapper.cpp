//File: C++CLIWrapper.cpp

#include "C++CLIWrapper.h"
#include <sstream>

namespace System { namespace Windows { namespace Interop
{
	CppCLI::CppCLI()
	{
		zServer = new ServerEngineWrapper();
	}

	CppCLI::!CppCLI()
	{
		if(zServer)	delete zServer, zServer = NULL;
	}

	String^ CppCLI::Init(int port, int nrOfPlayers, String^ gameMode, String^ mapName)
	{
		String^ returnText;
		char* gameModeName = nullptr;
		char* mapNameChar = nullptr;
		try
		{
			gameModeName = (char*)Marshal::StringToHGlobalAnsi(gameMode).ToPointer();
			mapNameChar = (char*)Marshal::StringToHGlobalAnsi(mapName).ToPointer();
		}
		catch(...)
		{
			return "Failed";
		}
		returnText = gcnew String(zServer->Init(port, nrOfPlayers, gameModeName, mapNameChar));

		return returnText;
	}

	unsigned int CppCLI::GetNumClients()
	{
		return zServer->GetNumClients();
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

	void CppCLI::SendMessageToClient( String^ message )
	{
		char* messageChar = nullptr;
		try
		{
			messageChar = (char*)Marshal::StringToHGlobalAnsi(message).ToPointer();
		}
		catch(...)
		{
		}
		zServer->SendMessageToClient(messageChar);
	}
}}}