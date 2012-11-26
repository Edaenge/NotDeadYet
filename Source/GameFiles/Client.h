/*
	Made by Jensen Christopher Datum(21/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "stdafx.h"
#include "Process.h"
#include "GraphicsEngine.h"
#include "Network/ServerChannel.h"
#include "Network/NewPlayerEvent.h"
#include "GameFiles/Player.h"

class Client : public MaloW::Process
{
private:
	string zIP;
	int	   zPort;
	GraphicsEngine* zEng;
	MaloW::ServerChannel* zServerChannel;
	MaloW::Array<Player*> zPlayers;
	float zTimeSinceLastPing;
private:
	/*! Handle Events */
	void HandleNewPlayerEvent(NewPlayerEvent* ev);
	void HandlePlayerUpdateEvent(PlayerUpdateEvent* ev);
	void HandlePingEvent(NetworkPacket* ev);
	/*! Handle Keyboard Input */
	void HandleKeyboardInput();
public:
	Client();
	/*! Connects to a Host with the specified parameters  */
	int Connect(std::string ip, int port);
	virtual ~Client();
	void Life();

	/*! Checks if Thread is alive */
	bool IsAlive();

	/*! Pings client to check if server is still running*/
	void Ping();
};