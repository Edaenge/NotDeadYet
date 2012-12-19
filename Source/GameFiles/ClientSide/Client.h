/*
	Made by Jensen Christopher Datum(21/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "Process.h"
#include "GraphicsEngine.h"
#include "Network/ServerChannel.h"
#include "GameFiles/Items/Inventory.h"
#include "GameFiles/Gui/InventoryGui.h"
#include "GameFiles/KeyUtil/KeyHandler.h"
#include "GameFiles/Gui/CircularListGui.h"
#include "Network/NetworkMessageConverter.h"
#include "GameFiles/ClientSide/WorldObjectManager.h"

using namespace MaloW;

class Client : public MaloW::Process
{
public:
	Client();
	/*! Connects to a Host with the specified parameters*/
	int Connect(const std::string& ip, const int port);
	virtual ~Client();
	void Life();
	/*! Checks if Thread is alive*/
	bool IsAlive();

private:
	/*! Handle Keyboard Input */
	void HandleKeyboardInput();
	/*! Splits Network Message and detects what type was sent*/
	void HandleNetworkMessage(const std::string& msg);
	/*! Pings client to check if server is still running*/
	void Ping();
	/*! Close the connection and print the reason to the client*/
	void CloseConnection(const std::string& reason);
	/*! Initializes the graphic stuff*/
	void InitGraphics();
	/*! Updates The Clock and returns the DeltaTime*/
	float Update();
	/*! Check if the key is pressed and potentially sends a message to the server*/
	bool CheckKey(const unsigned int ID);
	/*! Updates The Positions*/
	void UpdateWorldObjects();
	/*! Updates the camera position to follow the mesh*/
	void UpdateCameraPos();
	/*! Handles Message types*/
	void HandleUpdateObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id);
	void HandleNewObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id);
	void HandleRemoveObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id);
	/*! Send Camera Info and Rotation to Server*/
	void SendClientUpdate();
	/*! Checks Ray Vs Static Objects*/
	std::vector<StaticObject*> RayVsWorld();
	/*! Checks Mesh vs Mesh Collision*/
	void CheckCollision();

	void SendPickupItemMessage(unsigned int id);
	void SendDropItemMessage(unsigned int id);
	void HandleRemoveInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id);
	void HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id);
private:
	/*! Current Client ID*/
	int zID;
	int	zPort;
	bool zCreated;
	bool zRunning;
	std::string zIP;
	INT64 zStartime;
	float zDeltaTime;
	/*! Total Runtime*/
	float zFrameTime;
	float zSecsPerCnt;
	KeyHandler zKeyInfo;
	std::string zMeshID;
	GraphicsEngine* zEng;
	float zTimeSinceLastPing;
	/*! Counters*/
	float zSendUpdateDelayTimer;
	float zLootingGuiShowTimer;
	float zInventoryGuiShowTimer;
	ServerChannel* zServerChannel;
	WorldObjectManager zObjectManager;
	NetworkMessageConverter zMsgHandler;
	InventoryGui* zInvGui;
	CircularListGui* zCircularInvGui;
};