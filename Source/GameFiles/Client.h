//Made by Jensen Christopher 
//for project desperation* at Blekinge tekniska högskola.

#pragma once
#include "stdafx.h"
#include "Process.h"
#include "GraphicsEngine.h"
#include "Network/ServerChannel.h"

class Client : public MaloW::Process
{
private:
	string zIP;
	int	   zPort;
	GraphicsEngine* zEng;
	MaloW::ServerChannel* zServerChannel;
public:
	Client();
	/*! Connects to a Host with the specified parameters  */
	int Connect(std::string ip, int port);
	virtual ~Client();
	void Life();

};