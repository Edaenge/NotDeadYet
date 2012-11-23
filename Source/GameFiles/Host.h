/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "stdafx.h"
#include "Process.h"
#include "Network/ServerListener.h"
#include "Network/NetworkMessageConverter.h"

class Host : public MaloW::Process
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	int InitHost(int port, int maxClients);
	void Life();

private:
	/*Handles new incoming connections.*/
	void HandleNewConnections();

private:
	ServerListener* zServerListener;
	MaloW::Array<MaloW::ClientChannel*>* zClients;
	
	NetworkMessageConverter zMessageConverter;

	int	zMaxClients;

};
