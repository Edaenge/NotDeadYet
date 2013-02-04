//File: ServerEngineWrapper.h

#ifndef __SERVERENGINEWRAPPER_H__
#define __SERVERENGINEWRAPPER_H__

#include "../../../../Source/MaloWLib/Safe.h"

class ServerEngine;

#if defined _DLL_IMPORT
#define DLL_USAGE __declspec(dllimport)
#else
#define DLL_USAGE __declspec(dllexport)
#endif

class DLL_USAGE ServerEngineWrapper
{
private:
	ServerEngine* zServerEngine;

public:
	ServerEngineWrapper();
	~ServerEngineWrapper();

	const char* Init(const unsigned int &port, const unsigned int &maxPlayers) const;
	bool StartHost() const;
	void ShutdownHost() const;

	bool IsRunning() const;
	int GetNrOfPlayers() const;
	char* ProcessText(char* msg);

};



#endif
