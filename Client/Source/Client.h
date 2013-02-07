/*
	Made by Jensen Christopher Datum(21/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include <Process.h>
#include <Safe.h>
#include <GraphicsEngine.h>
#include <ServerChannel.h>
#include <Inventory.h>
#include "KeyHandler.h"
#include "NetworkMessageConverter.h"
#include "WorldObjectManager.h"
#include "GuiManager.h"
#include <World/WorldRenderer.h>

using namespace MaloW;

struct Looting_Data
{
	Looting_Data()
	{
		owner = -1;
		gid = Gui_Item_Data();
		//type = OBJECT_TYPE_STATIC_OBJECT;
	}
	int owner;
	Gui_Item_Data gid;
	/*
		DYNAMIC, STATIC, DEAD_PLAYER
	*/
	int type;
};

class Client : public MaloW::Process, public Observer
{
public:
	Client();
	/*! Connects to a Host with the specified parameters*/
	void Connect(const std::string& IPAddress, const unsigned int &port);
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
	void ReadMessages();
	/*! Sends an acknowledge, important message received.*/
	void SendAck(unsigned int IM_ID);

	//////////////////////
	//					//
	//	   Input		//
	//			  		//
	//////////////////////

	void CheckMovementKeys();
	/*! Handle Keyboard Input */
	void HandleKeyboardInput();
	/*! Used to Equip weapons with Keyboard*/
	void HandleWeaponEquips();
	/*! Check if the key is pressed and potentially sends a message to the server*/
	bool CheckKey(const unsigned int ID);
	void HandleDebugInfo();
	//////////////////////
	//					//
	//	   Updates		//
	//			  		//
	//////////////////////

	/*! Updates The Positions*/
	void UpdateActors();
	/*! Updates the camera position to follow the mesh.*/
	void UpdateCameraPos();
	/*! Updates The Clock and returns the DeltaTime*/
	float Update();

	/*! Checks Ray Vs Static/Dynamic Objects*/
	std::vector<Looting_Data> RayVsWorld();
	/*! Checks PlayerMesh vs WorldMesh Collision*/
	bool CheckCollision();

	//////////////////////
	//					//
	//	   Actors		//
	//			  		//
	//////////////////////
	//Temporary Code
	bool UpdateActor(const std::vector<std::string>& msgArray, const long ID);
	bool RemoveActor(const long ID);

	/*! Adds A Player Object.*/
	bool AddActor(const std::vector<std::string>& msgArray, const long ID);
	

	void HandleDeadPlayerMessage(const int ID);
	//////////////////////
	//					//
	//		Items		//
	//			  		//
	//////////////////////

	bool CreateItemFromMessage( std::vector<std::string> msgArray, int& Index, Item*& item, const long ID);
	void SendLootItemMessage(const long ID, const long ItemID, const int TYPE);
	void SendPickupItemMessage(const long ID);
	void SendDropItemMessage(const long ID);
	void SendUseItemMessage(const long ID);
	void HandleRemoveInventoryItem(const long ID);
	void HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned long id);
	/*! Uses the Selected Item*/
	void HandleUseItem(const long ID);
	void DisplayMessageToClient(const std::string& msg);
	void HandleEquipItem(const long ItemID, const int Slot);
	bool HandleUnEquipItem(const long ItemID, const int Slot);
	void SendUnEquipItem(const long ID, const int Slot);
	void HandleRemoveEquipment(const long ItemID, const int Slot);
	void HandeRemoveDeadPlayerItem(const long ObjID, const long ItemID, const int type);
	void HandleWeaponUse(const long ID);

protected:
	virtual void onEvent(Event* e);

private:
	/*! Current Client ID*/
	unsigned long zID;
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

	World* zWorld;
	WorldRenderer* zWorldRenderer;
	WorldAnchor* zAnchor;
	iImage* zCrossHair;
	
	bool zGameStarted;
};