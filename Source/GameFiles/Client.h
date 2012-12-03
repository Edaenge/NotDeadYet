/*
	Made by Jensen Christopher Datum(21/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "Process.h"
#include "GraphicsEngine.h"
#include "GameFiles/Player.h"
#include "GameFiles/KeyHandler.h"
#include "Network/ServerChannel.h"
#include "Network/NetworkMessageConverter.h"

class Client : public MaloW::Process
{
private:
	int zID;

	int	zPort;
	std::string zIP;

	INT64 zStartime;
	float zDeltaTime;
	float zSecsPerCnt;

	KeyHandler zKeyInfo;
	std::string zMeshID;
	GraphicsEngine* zEng;

	float zTimeSinceLastPing;

	std::vector<Player*> zPlayers;
	NetworkMessageConverter zMsgHandler;
	MaloW::ServerChannel* zServerChannel;
private:
	/*! Handle Keyboard Input */
	void HandleKeyboardInput();
	/*! Splits Network Message and detects what type was sent*/
	void HandleNetworkMessage(std::string msg);
	/*! Handles Message type*/
	void HandleNewPlayer(const std::vector<std::string>& msgArray);
	void HandlePlayerUpdate(const std::vector<std::string>& msgArray);
	void HandleRemovePlayer(const std::vector<std::string>& msgArray);
	/*! Pings client to check if server is still running*/
	void Ping();
	/*! Close the connection and print the reason to the client*/
	void CloseConnection(const std::string reason);
	/*! Initializes the graphic stuff*/
	void initClient();
	float Update();
	void CheckKey(unsigned int ID);
public:
	Client();
	/*! Connects to a Host with the specified parameters*/
	int Connect(const std::string& ip, const int port);
	virtual ~Client();
	void Life();
	/*! Checks if Thread is alive*/
	bool IsAlive();
};