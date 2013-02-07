//File: ServerEngineWrapper.h

#ifndef __SERVERENGINEWRAPPER_H__
#define __SERVERENGINEWRAPPER_H__

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

	const char* Init(const unsigned int &port, const unsigned int &maxPlayers, char* gameMode, char* mapName) const;
	bool StartHost() const;
	void ShutdownHost() const;

	bool IsRunning() const;
	unsigned int GetNumClients() const;

};



#endif
