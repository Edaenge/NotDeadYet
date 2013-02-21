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
#include "ClientActorManager.h"
#include "GuiManager.h"
#include <World/WorldRenderer.h>
#include <Packets/ServerFramePacket.h>
#include <Packets/Packet.h>
#include "InGameMenu.h"
#include "PickAnimalMenu.h"

using namespace MaloW;

enum CLIENT_ACTOR_TYPE
{
	NONE,
	HUMAN,
	GHOST,
	ANIMAL

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

	void SetBlackImage(iImage* image){ this->zBlackImage = image; }

private:

	//////////////////////
	//					//
	//	   Init			//
	//			  		//
	//////////////////////

	/*! Initiates all the Client Data*/
	void Init();

	/*! Initializes the graphic stuff*/
	void InitGraphics(const std::string& mapName);

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
	// Handle A Network Packet
	void HandleNetworkPacket(Packet* P);
	/*! Splits Network Message and detects what type was sent*/
	void HandleNetworkMessage(const std::string& msg);
	/*! Reads Messages from the server*/
	void ReadMessages();

	//////////////////////
	//					//
	//	   Input		//
	//			  		//
	//////////////////////

	void CheckAnimalInput();
	void CheckGhostSpecificKeys();
	void CheckNonGhostInput();
	void CheckPlayerSpecificKeys();
	void CheckMovementKeys();
	/*! Handle Keyboard Input */
	void CheckKeyboardInput();
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
	void UpdateMeshRotation();
	/*! Updates The Clock and returns the DeltaTime*/
	float Update();

	/*! Checks Ray Vs Static/Dynamic Objects*/
	std::vector<unsigned int> RayVsWorld();
	/*! Checks PlayerMesh vs WorldMesh Collision*/
	bool CheckCollision();

	void UpdateCameraOffset(unsigned int state);

	//////////////////////
	//					//
	//	   Actors		//
	//			  		//
	//////////////////////
	//Temporary Code
	void UpdateActors(ServerFramePacket* SFP);
	bool RemoveActor(const unsigned int ID);
	bool HandleTakeDamage(const unsigned int ID, float damageTaken);
	/*! Adds A Player Object.*/
	bool AddActor(const std::vector<std::string>& msgArray, const unsigned int ID);
	

	void HandleDeadPlayerMessage(const int ID);
	//////////////////////
	//					//
	//		Items		//
	//			  		//
	//////////////////////

	bool CreateItemFromMessage( std::vector<std::string> msgArray, int& Index, Item*& item, const unsigned int ID);
	void SendLootItemMessage(const unsigned int ID, const unsigned int ItemID, const int Type, const int SubType);
	void SendPickupItemMessage(const unsigned int ID);
	void SendDropItemMessage(const unsigned int ID);
	void SendUseItemMessage(const unsigned int ID);
	void SendCraftItemMessage(const unsigned int ID);
	void HandleRemoveInventoryItem(const unsigned int ID);
	void HandleAddInventoryItem(const std::vector<std::string>& msgArray);
	/*! Uses the Selected Item*/
	void HandleUseItem(const unsigned int ID);
	void DisplayMessageToClient(const std::string& msg);
	void HandleEquipItem(const unsigned int ItemID, const int Slot);
	bool HandleUnEquipItem(const unsigned int ItemID, const int Slot);
	void SendUnEquipItem(const unsigned int ID, const int Slot);
	void SendEquipItem(const unsigned int ID);
	void HandleRemoveEquipment(const unsigned int ItemID, const int Slot);
	void HandleWeaponUse(const unsigned int ID);
	void HandleDisplayLootData(std::vector<std::string> msgArray);
protected:
	virtual void OnEvent(Event* e);

private:
	/*! Current Client ID*/
	unsigned int zID;
	int	zPort;

	INT64 zStartime;
	float zDeltaTime;
	/*! Total Runtime*/
	float zFrameTime;
	float zSecsPerCnt;
	float zTimeSinceLastPing;
	/*! Counters*/
	float zSendUpdateDelayTimer;

	unsigned int zActorType;
	bool zCreated;
	bool zRunning;
	bool zShowCursor;
	
	std::string zIP;
	std::string zMeshID;
	std::string zName;

	KeyHandler zKeyInfo;
	GraphicsEngine* zEng;
	GuiManager* zGuiManager;
	ServerChannel* zServerChannel;
	ClientActorManager* zActorManager;
	NetworkMessageConverter zMsgHandler;
	Inventory* zPlayerInventory;

	World* zWorld;
	WorldRenderer* zWorldRenderer;
	WorldAnchor* zAnchor;
	iImage* zCrossHair;
	
	iImage* zDamageIndicator;
	float	zDamageOpacity;
	
	bool zGameStarted;

	iImage* zBlackImage;
	InGameMenu* zIgm;
	PickAnimalMenu* zPam;

	//Player Data
	float zHealth;
	float zStamina;
	float zHunger;
	float zHydration;

	Vector3 zMeshOffset;
	std::map<std::string, Vector3> zMeshCameraOffsets;
	std::map<unsigned int, Vector3> zStateCameraOffset;
};