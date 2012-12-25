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
#include "GameFiles/ClientSide/GuiManager.h"

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
	/*! Checks if Cursor should be Visible*/
	bool GetCursorVisibility();

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
	/*! Updates the camera position to follow the mesh.*/
	void UpdateCameraPos();
	/*! Handles Message types.*/
	void HandleUpdateObjectMessage(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id);
	void HandleNewObjectMessage(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id);
	void HandleRemoveObjectMessage(const unsigned int objectType, const unsigned int id);
	
	/*! Updates A Static Object.*/
	void UpdateStaticObjects(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Updates An Animal Object.*/
	void UpdateAnimalObjects(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Updates A Dynamic Object.*/
	void UpdateDynamicObjects(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Updates A Player Object*/
	void UpdatePlayerObjects(const std::vector<std::string>& msgArray, const unsigned int id);
	
	/*! Removes A Player Object*/
	void RemovePlayerObject(const unsigned int id);
	/*! Removes An Animal Object*/
	void RemoveAnimalObject(const unsigned int id);
	/*! Removes A Static Object*/
	void RemoveStaticObject(const unsigned int id);
	/*! Removes A Dynamic Object*/
	void RemoveDynamicObject(const unsigned int id);

	/*! Adds A Player Object*/
	void AddNewPlayerObject(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Adds An Animal Object*/
	void AddNewAnimalObject(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Adds A Static Object*/
	void AddNewStaticObject(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Adds A Dynamic Object*/
	void AddNewDynamicObject(const std::vector<std::string>& msgArray, const unsigned int id);

	/*! Send Camera Info and Rotation to Server*/
	void SendClientUpdate();
	/*! Checks Ray Vs Static/Dynamic Objects*/
	std::vector<WorldObject*>& RayVsWorld();
	/*! Checks PlayerMesh vs WorldMesh Collision*/
	void CheckCollision();

	void SendPickupItemMessage(unsigned int id);
	void SendDropItemMessage(unsigned int id);
	void HandleRemoveInventoryItem(const unsigned int id);
	void HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Initiates all the Client Data*/
	void Init();
	/*! Reads Messages from the server*/
	void ReadMessage();

	void CheckMovementKeys();
private:
	/*! Current Client ID*/
	int zID;
	int	zPort;

	INT64 zStartime;
	float zDeltaTime;
	/*! Total Runtime*/
	float zFrameTime;
	float zSecsPerCnt;
	float zTimeSinceLastPing;
	/*! Counters*/
	float zSendUpdateDelayTimer;

	bool zCreated;
	bool zRunning;
	bool zShowCursor;

	std::string zIP;
	std::string zMeshID;

	KeyHandler zKeyInfo;
	GraphicsEngine* zEng;
	GuiManager* zGuiManager;
	ServerChannel* zServerChannel;
	WorldObjectManager* zObjectManager;
	NetworkMessageConverter zMsgHandler;
};