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

struct TempImage 
{
	iImage* image;
	int id;
};

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
	/*! Updates A Static Object.*/
	bool UpdateStaticObjects(const std::vector<std::string>& msgArray, const int id);
	/*! Updates An Animal Object.*/
	bool UpdateAnimalObjects(const std::vector<std::string>& msgArray, const int id);
	/*! Updates A Dynamic Object.*/
	bool UpdateDynamicObjects(const std::vector<std::string>& msgArray, const int id);
	/*! Updates A Player Object*/
	bool UpdatePlayerObjects(const std::vector<std::string>& msgArray, const int id);
	
	/*! Removes A Player Object*/
	bool RemovePlayerObject(const int id);
	/*! Removes An Animal Object*/
	bool RemoveAnimalObject(const int id);
	/*! Removes A Static Object*/
	bool RemoveStaticObject(const int id);
	/*! Removes A Dynamic Object*/
	bool RemoveDynamicObject(const int id);

	/*! Adds A Player Object*/
	bool AddNewPlayerObject(const std::vector<std::string>& msgArray, const int id);
	/*! Adds An Animal Object*/
	bool AddNewAnimalObject(const std::vector<std::string>& msgArray, const int id);
	/*! Adds A Static Object*/
	bool AddNewStaticObject(const std::vector<std::string>& msgArray, const int id);
	/*! Adds A Dynamic Object*/
	bool AddNewDynamicObject(const std::vector<std::string>& msgArray, const int id);

	/*! Send Camera Info and Rotation to Server*/
	void SendClientUpdate();
	/*! Checks Ray Vs Static/Dynamic Objects*/
	std::vector<Gui_Item_Data> RayVsWorld();
	/*! Checks PlayerMesh vs WorldMesh Collision*/
	bool CheckCollision();

	void SendPickupItemMessage(const int id);
	void SendDropItemMessage(const int id);
	void HandleRemoveInventoryItem(const int id);
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
	Inventory* zPlayerInventory;
	std::vector<TempImage> images;
};