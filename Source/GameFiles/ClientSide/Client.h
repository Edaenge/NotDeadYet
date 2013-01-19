/*
	Made by Jensen Christopher Datum(21/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "Process.h"
#include "../../MaloWLib/Safe.h"
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
	int Connect(const std::string& IP, const int PORT);
	virtual ~Client();
	void Life();
	/*! Checks if Thread is alive*/
	bool IsAlive();
	/*! Checks if Cursor should be Visible*/
	bool GetCursorVisibility();

private:

	//////////////////////
	//					//
	//	   Init			//
	//			  		//
	//////////////////////

	/*! Initiates all the Client Data*/
	void Init();

	/*! Initializes the graphic stuff*/
	void InitGraphics();


	//////////////////////
	//					//
	//	  Messaging		//
	//			  		//
	//////////////////////

	/*! Pings client to check if server is still running*/
	void Ping();
	/*! Close the connection and print the reason to the client*/
	void CloseConnection(const std::string& reason);
	/*! Send Camera Info and Rotation to Server*/
	void SendClientUpdate();
	/*! Splits Network Message and detects what type was sent*/
	void HandleNetworkMessage(const std::string& msg);
	/*! Reads Messages from the server*/
	void ReadMessage();

	//////////////////////
	//					//
	//	   Input		//
	//			  		//
	//////////////////////

	void CheckMovementKeys();
	/*! Handle Keyboard Input */
	void HandleKeyboardInput();
	/*! Check if the key is pressed and potentially sends a message to the server*/
	bool CheckKey(const unsigned int ID);

	//////////////////////
	//					//
	//	   Updates		//
	//			  		//
	//////////////////////
	/*! Updates The Positions*/
	void UpdateWorldObjects();
	/*! Updates the camera position to follow the mesh.*/
	void UpdateCameraPos();
	/*! Updates The Clock and returns the DeltaTime*/
	float Update();

	/*! Checks Ray Vs Static/Dynamic Objects*/
	std::vector<Gui_Item_Data> RayVsWorld();
	/*! Checks PlayerMesh vs WorldMesh Collision*/
	bool CheckCollision();

	

	//////////////////////
	//					//
	//	   Objects		//
	//			  		//
	//////////////////////
	/*! Updates A Static Object.*/
	bool UpdateStaticObjects(const std::vector<std::string>& msgArray, const int ID);
	/*! Updates An Animal Object.*/
	bool UpdateAnimalObjects(const std::vector<std::string>& msgArray, const int ID);
	/*! Updates A Dynamic Object.*/
	bool UpdateDynamicObjects(const std::vector<std::string>& msgArray, const int ID);
	/*! Updates A Player Object*/
	bool UpdatePlayerObjects(const std::vector<std::string>& msgArray, const int ID);
	
	/*! Removes A Player Object*/
	bool RemovePlayerObject(const int ID);
	/*! Removes An Animal Object*/
	bool RemoveAnimalObject(const int ID);
	/*! Removes A Static Object*/
	bool RemoveStaticObject(const int ID);
	/*! Removes A Dynamic Object*/
	bool RemoveDynamicObject(const int ID);

	/*! Adds A Player Object*/
	bool AddNewPlayerObject(const std::vector<std::string>& msgArray, const int ID);
	/*! Adds An Animal Object*/
	bool AddNewAnimalObject(const std::vector<std::string>& msgArray, const int ID);
	/*! Adds A Static Object*/
	bool AddNewStaticObject(const std::vector<std::string>& msgArray, const int ID);
	/*! Adds A Dynamic Object*/
	bool AddNewDynamicObject(const std::vector<std::string>& msgArray, const int ID);

	//////////////////////
	//					//
	//		Items		//
	//			  		//
	//////////////////////

	void SendPickupItemMessage(const int ID);
	void SendDropItemMessage(const int ID);
	void HandleRemoveInventoryItem(const int ID);
	void HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id);
	/*! Uses the Selected Item*/
	void HandleUseItem(const int ID);
	void DisplayMessageToClient(const std::string& msg);
	void HandleEquipItem(const int ItemID, const int Slot);
	void HandleUnEquipItem(const int ItemID, const int Slot);
	void SendUnEquipItem(const int ID, const int Slot);
	void HandleRemoveEquipment(const int ItemID, const int Slot);
	void HandleWeaponUse(const int ID);
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

	bool zIsHuman;
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
};