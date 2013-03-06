#include "Host.h"
#include "ClientServerMessages.h"
#include "ClientConnectedEvent.h"
#include "ClientDisconnectedEvent.h"
#include "ClientDroppedEvent.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include <algorithm>

// 50 updates per sec
static const float UPDATE_DELAY = 0.020f;

Host::Host() :
	zSynchronizer(0),
	zGame(0)
{
	MaloW::ClearDebug();
	Messages::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): Server: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): Server: vld.h included.");
#endif

	this->zGameTimer = new GameTimer();

	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zMinClients = 0;
	this->zDeltaTime = 0.0f;
	this->zTimeOut = 10.0f;
	this->zPingMessageInterval = 2.5f;
	this->zTimeSinceLastPing = 0.0f;
	this->zSendUpdateDelayTimer = 0.0f;
	this->zGameStarted = false;
	this->zRestartRequested = false;
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();

	this->Close();
	this->WaitUntillDone();

	SAFE_DELETE(this->zGame);
	SAFE_DELETE(this->zGameTimer);

	SAFE_DELETE(this->zSynchronizer);

	SAFE_DELETE(this->zServerListener);

	for (auto it = this->zClients.begin(); it != this->zClients.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	this->zClients.clear();

	/*Free any still existing Event.*/
	unsigned int counter = 0;
	unsigned int nrOfMessages = this->GetEventQueueSize();
	MaloW::ProcessEvent* pe;

	while (counter < nrOfMessages)
	{
		pe = PeekEvent();
		SAFE_DELETE( pe );

		counter++;
	}

	FreePhysics();
}

void Host::SendMessageToClient( const std::string& message )
{
	if (message.find("EV") == 0)
	{
		std::string msg = this->zMessageConverter.ConvertStringToSubstring("EV", message, true);

		std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);
		if (msg.find("restart") == 0)
		{
			this->zRestartRequested = true;
		}
	}
	else
	{
		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, message);

		this->SendToAllClients(msg, true);
	}
}

void Host::Life()
{
	if (Messages::FileWrite())
		Messages::Debug("Host Process Started");

	this->zServerListener->Start();

	static const float FRAME_TIME = 240.0f;
	static const float TARGET_DT = 1.0f / FRAME_TIME;

	this->zGameTimer->Init();
	while(this->stayAlive)
	{
		this->zDeltaTime = this->zGameTimer->Frame();

		this->UpdateGame();
		
		if (FRAME_TIME > 0)
		{
			if (this->zDeltaTime < TARGET_DT)
			{
				DWORD sleepTime = (TARGET_DT - this->zDeltaTime) * 1000.0f;
				Sleep(sleepTime);
			}
		}
	}
}

void Host::UpdateGame()
{
	ReadMessages();

	//if (!this->zGameStarted)
	//{
	//	//Check if All players Are Ready
	//	unsigned int nrOfReadyPlayers = 0;
	//	for (auto it = this->zClients.begin(); it != this->zClients.end(); it++)
	//	{
	//		if (it->second->GetReady())
	//		{
	//			nrOfReadyPlayers++;
	//		}
	//	}
	//	if (nrOfReadyPlayers == this->zClients.size())
	//	{
	//		this->zGameStarted = true;
	//	}
	//}
	//else
	//{
	if ( !this->zGame )
	{

	}
	else if(this->zGame->Update(this->zDeltaTime))
	{
		this->PingClients();

		this->zSendUpdateDelayTimer += this->zDeltaTime;

		if (zSendUpdateDelayTimer >= UPDATE_DELAY)
		{
			this->SynchronizeAll();

			this->zSendUpdateDelayTimer = 0.0f;

			this->SendToAllClients(this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_UPDATES_PER_SEC, (float)this->zGameTimer->GetFPS()), false);
			
		}
	}
	else
	{
		
	}

	if (this->zRestartRequested)
		this->Restart(zGameMode, zMapName);

	//}
}

const char* Host::InitHost(const unsigned int &port, const unsigned int &maxClients,  const std::string& gameModeName, const std::string& mapName)
{
	this->zMaxClients = maxClients;

	//Restart(gameModeName, mapName);
	
	FreePhysics();
	PhysicsInit();
	
	//Init ActorSycnher
	if ( this->zSynchronizer ) 
	{
		delete zSynchronizer;
		this->zSynchronizer->ClearAll();
	}

	this->zSynchronizer = new ActorSynchronizer();
	

	//Init game
	if(this->zGame)
	{
		this->RemoveObserver(zGame);
		delete zGame;
	}

	this->zGame = new Game(maxClients, zSynchronizer, gameModeName, mapName);
	this->AddObserver(zGame);
	
	//server Listener
	if ( !zServerListener )
	{
		try
		{
			zServerListener = new ServerListener(this, port);
			zServerListener->Start();
		}
		catch(const char *str)
		{
			return str;
		}
	}

	return 0;
}

void Host::SendToAllClients(const std::string& message, bool bImportant)
{
	if(!HasClients())
		return;

	for (auto it = zClients.begin(); it != zClients.end(); it++)
	{
		if (bImportant)
		{
			it->first->Send(message);
		}
		else
		{
			it->first->TrySend(message);
		}
		
	}
}

bool Host::HasClients() const
{
	return !zClients.empty();
}

unsigned int Host::GetNumClients() const
{
	return zClients.size();
}

void Host::ReadMessages()
{
	static unsigned int MAX_MESSAGES_TO_READ = 10000;
	unsigned int nrOfMessages = this->GetEventQueueSize();

	// No new messages
	if(nrOfMessages == 0)
		return;

	nrOfMessages = min(nrOfMessages, MAX_MESSAGES_TO_READ);
	MaloW::ProcessEvent* pe;

	for (unsigned int i = 0; i < nrOfMessages; i++)
	{
		pe = PeekEvent();

		if ( ClientConnectedEvent* CCE = dynamic_cast<ClientConnectedEvent*>(pe) )
		{
			// A Client Connected
			HandleNewConnection(CCE->GetClientChannel());
		}
		else if ( MaloW::NetworkPacket* NP = dynamic_cast<MaloW::NetworkPacket*>(pe) )
		{
			HandleReceivedMessage(dynamic_cast<MaloW::ClientChannel*>(NP->GetChannel()), NP->GetMessage());
		}
		else if ( ClientDisconnectedEvent* CDE = dynamic_cast<ClientDisconnectedEvent*>(pe) )
		{
			HandleClientDisconnect(CDE->GetClientChannel());
		}

		// Unhandled Message
		SAFE_DELETE(pe);
	}
}

void Host::HandleReceivedMessage( MaloW::ClientChannel* cc, const std::string &message )
{
	std::vector<std::string> msgArray;
	if(message.empty())
		return;

	msgArray = this->zMessageConverter.SplitMessage(message); 
	ClientData* cd = zClients[cc];
	
	// Empty Array
	if ( msgArray.size() == 0 ) 
		return;

	//Sets last received packet time from this client.
	cd->SetLastPacketTime(this->zGameTimer->GetRunTime());

	//Handles updates from client.
	if(msgArray[0].find(M_CLIENT_DATA.c_str()) == 0)
	{
		this->HandleClientUpdate(msgArray, cd);
	}
	//Handles key presses from client.
	else if(msgArray[0].find(M_KEY_DOWN.c_str()) == 0)
	{
		KeyDownEvent e;
		int dKey = this->zMessageConverter.ConvertStringToInt(M_KEY_DOWN, msgArray[0]);

		e.clientData = cd;
		e.key = dKey;
		NotifyObservers(&e);
	}
	//Handles key releases from client.
	else if(msgArray[0].find(M_KEY_UP.c_str()) == 0)
	{
		KeyUpEvent e;
		int uKey = this->zMessageConverter.ConvertStringToInt(M_KEY_UP, msgArray[0]);

		e.clientData = cd;
		e.key = uKey;
		NotifyObservers(&e);
	}
	//Handles Pings from client.
	else if(msgArray[0].find(M_PING.c_str()) == 0)
	{
		float latency = this->zMessageConverter.ConvertStringToFloat(M_PING, msgArray[0]);
		
		latency = (this->zGameTimer->GetRunTime() - latency) * 0.5f;

		cd->AddLatency(latency);
		latency = cd->GetAverageLatency();

		latency *= 1000.0f;

		cd->Send(this->zMessageConverter.Convert(MESSAGE_TYPE_CLIENT_LATENCY, latency));
	}
	//Handles ready from client.
	else if(msgArray[0].find(M_READY_PLAYER.c_str()) == 0)
	{
		UserReadyEvent e;
		e.clientData = cd;
		NotifyObservers(&e);
	}
	//Handles Swap weapon message
	else if(msgArray[0].find(M_WEAPON_EQUIPMENT_SWAP.c_str()) == 0)
	{
		PlayerSwapEquippedWeaponsEvent e;

		e.clientdData = cd;
		NotifyObservers(&e);
	}
	//Handle Item usage in Inventory
	else if(msgArray[0].find(M_ITEM_USE.c_str()) == 0)
	{
		PlayerUseItemEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_USE, msgArray[0]);

		e.clientData = cd;
		e.itemID = _itemID;
		NotifyObservers(&e);
	}
	//Handle Item crafting in Inventory
	else if(msgArray[0].find(M_ITEM_CRAFT.c_str()) == 0)
	{
		if (msgArray.size() > 2)
		{
			PlayerCraftItemEvent e;
			int _itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_CRAFT, msgArray[0]);
			unsigned int _itemType = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[1]);
			unsigned int _itemSubType = this->zMessageConverter.ConvertStringToInt(M_ITEM_SUB_TYPE, msgArray[2]);
			e.clientData = cd;
			e.itemID = _itemID;
			e.craftedItemType = _itemType;
			e.craftedItemSubType = _itemSubType;
			NotifyObservers(&e);
		}
	}
	//Handle Equip Item
	else if (msgArray[0].find(M_EQUIP_ITEM.c_str()) == 0)
	{
		PlayerEquipItemEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_EQUIP_ITEM, msgArray[0]);

		e.itemID = _itemID;
		e.clientData = cd;
		NotifyObservers(&e);
	}
	//Handle UnEquip Item in Equipment
	else if(msgArray[0].find(M_UNEQUIP_ITEM.c_str()) == 0)
	{
		PlayerUnEquipItemEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
		int _eq_Slot = -1;

		if (msgArray.size() > 1)
		{
			_eq_Slot = this->zMessageConverter.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
		}

		e.eq_Slot = _eq_Slot;
		e.itemID = _itemID;
		e.clientData = cd;
		NotifyObservers(&e);
	}
	//Handles Equipped Weapon usage
	else if(msgArray[0].find(M_WEAPON_USE.c_str()) == 0)
	{
		PlayerUseEquippedWeaponEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);

		e.clientData = cd;
		e.itemID = _itemID;
		NotifyObservers(&e);
	}
	else if(msgArray[0].find(M_LOOT_OBJECT.c_str()) == 0)
	{
		PlayerLootObjectEvent e;
		e.clientData = cd;
		for (auto it = msgArray.begin(); it != msgArray.end(); it++)
		{
			unsigned int id = this->zMessageConverter.ConvertStringToInt(M_LOOT_OBJECT, (*it));

			e.actorID.push_back(id);
		}
		NotifyObservers(&e);
	}
	//Handles loot item request
	else if(msgArray[0].find(M_LOOT_ITEM.c_str()) == 0)
	{
		HandleLootRequest(msgArray, cd);
	}
	//Handles Drop Item Requests from Client
	else if(msgArray[0].find(M_DROP_ITEM.c_str()) == 0)
	{
		PlayerDropItemEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_DROP_ITEM, msgArray[0]);

		e.clientData = cd;
		e.itemID = _itemID;
		NotifyObservers(&e);
	}
	else if(msgArray[0].find(M_PLAY_AS_ANIMAL.c_str()) == 0)
	{
		PlayerAnimalSwapEvent e;
		int animalType = this->zMessageConverter.ConvertStringToInt(M_PLAY_AS_ANIMAL, msgArray[0]);

		e.zActor = NULL;
		e.clientData = cd;
		e.zAnimalType = animalType;
		NotifyObservers(&e);
	}
	else if (msgArray[0].find(M_ANIMAL_ATTACK.c_str()) == 0)
	{
		unsigned int mouseButton = this->zMessageConverter.ConvertStringToInt(M_ANIMAL_ATTACK, msgArray[0]);

		PlayerAnimalAttackEvent e;
		e.clientData = cd;
		e.mouseButton = mouseButton;
		NotifyObservers(&e);
	}
	else if(msgArray[0].find(M_DEER_EAT_OBJECT.c_str()) == 0)
	{
		PlayerDeerEatObjectEvent e;

		e.clientData = cd;
		for (auto it = msgArray.begin(); it != msgArray.end(); it++)
		{
			unsigned int id = this->zMessageConverter.ConvertStringToInt(M_DEER_EAT_OBJECT, (*it));

			e.actorID.push_back(id);
		}

		NotifyObservers(&e);
	}
	else if(msgArray[0].find(M_LEAVE_ANIMAL.c_str()) == 0)
	{
		PlayerLeaveAnimalEvent e;

		e.clientData = cd;

		NotifyObservers(&e);
	}
	else if(msgArray[0].find(M_ACTOR_KILL.c_str()) == 0)
	{
		PlayerKillEvent e;

		e.clientData = cd;
		NotifyObservers(&e);
	}
	//Handles user data from client. Used when the player is new.
	else if(msgArray[0].find(M_USER_DATA.c_str()) == 0)
	{
		HandleUserData(msgArray, cd);
	}
	else if(msgArray[0].find(M_RESTART_GAME_REQUEST.c_str()) == 0)
	{
		this->zRestartRequested = true;
	}
	//Handles if client disconnects.
	else if(msgArray[0].find(M_CONNECTION_CLOSED.c_str()) == 0)
	{
		HandleClientDisconnect(cc);
	}
	//Handles if not of the above.
	else
	{
		MaloW::Debug("Warning: The host cannot handle the message \"" + message + "\" in HandleReceivedMessages.");
	}
	
}

void Host::BroadCastServerShutdown()
{
	std::string mess = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_SHUTDOWN);
	SendToAllClients(mess, true);
}

void Host::PingClients()
{
	if(!HasClients())
		return;

	if( this->zTimeSinceLastPing < this->zPingMessageInterval )
	{
		this->zTimeSinceLastPing += this->zDeltaTime;

		return;
	}

	this->zTimeSinceLastPing = 0.0f;
	std::string message;
	for(auto it = zClients.begin(); it != zClients.end(); it++)
	{
		(*it).first->TrySend( zMessageConverter.Convert(MESSAGE_TYPE_PING, this->zGameTimer->GetRunTime()) );
	}
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::HandleClientDisconnect( MaloW::ClientChannel* channel )
{
	//DON'T call ClientChannel->Kick in here! For fuck sakes
	//The kick command will push a ClientDisonnected event, again.

	if( !channel )
		return;

	PlayerDisconnectedEvent e;
	auto i = zClients.find(channel);

	if( i == zClients.end() )
		return;
	

	ClientData* cd = i->second;
	e.clientData = cd;

	NotifyObservers(&e);
	delete cd, cd = NULL;
	zClients.erase(i);
}

void Host::HandleNewConnection( MaloW::ClientChannel* CC )
{
	zClientChannels.insert(CC);

	ClientData* cd = new ClientData(CC);
	zClients[CC] = cd;
	CC->Start();

	PlayerConnectedEvent e;
	e.clientData = cd;
	NotifyObservers(&e);
}

void Host::HandleClientUpdate(const std::vector<std::string> &msgArray, ClientData* cd)
{
	ClientDataEvent e;
	e.clientData = cd;

	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		char key[512];
		sscanf_s((*it).c_str(), "%s ", key, sizeof(key));

		if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			e.direction = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it));
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			e.upVector = this->zMessageConverter.ConvertStringToVector(M_UP, (*it));
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			e.rotation = this->zMessageConverter.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else
		{
			MaloW::Debug("Unknown message in HandleClientUpdate.");
		}
	}

	NotifyObservers(&e);

	//Update Latency
// 	float latency;
// 
// 	if(!cd->CalculateLatency(latency))
// 		return;
// 
// 	pl->SetLatency(latency);
}

void Host::HandleLootRequest( const std::vector<std::string> &msgArray, ClientData* cd )
{
	PlayerLootItemEvent e;
	unsigned int _objID = 0;
	unsigned int _itemID = 0;
	int _type = 20;
	int _subType = 20;
	if (msgArray.size() > 3)
	{
		
		_objID = this->zMessageConverter.ConvertStringToInt(M_LOOT_ITEM, msgArray[0]);
		_itemID = this->zMessageConverter.ConvertStringToInt(M_OBJECT_ID, msgArray[1]);
		_type = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);
		_subType = this->zMessageConverter.ConvertStringToInt(M_ITEM_SUB_TYPE, msgArray[3]);
		e.clientData = cd;
		e.itemID = _itemID;
		e.objID = _objID;
		e.itemType = _type;
		e.subType = _subType;
		NotifyObservers(&e);
	}
	else
	{
		MaloW::Debug("Msg array size is to short size: " + MaloW::convertNrToString((float)msgArray.size()) + " Expected size 3");
	}
}

void Host::HandleUserData( const std::vector<std::string> &msgArray, ClientData* cd )
{
	UserDataEvent e;
	e.clientData = cd;

	for (auto it_m = msgArray.begin() + 1; it_m < msgArray.end(); it_m++)
	{
		if(it_m->find(M_MESH_MODEL) == 0)
		{
			e.playerModel = this->zMessageConverter.ConvertStringToSubstring(M_MESH_MODEL, (*it_m));
		}
		else if(it_m->find(M_DIRECTION) == 0)
		{
			e.direction = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it_m));
		}
		else if(it_m->find(M_USER_NAME) == 0)
		{
			e.playerName = this->zMessageConverter.ConvertStringToSubstring(M_USER_NAME, (*it_m), true);
		}
		else if(it_m->find(M_UP) == 0)
		{
			e.upVector = this->zMessageConverter.ConvertStringToVector(M_UP, (*it_m));
		}
	}

	NotifyObservers(&e);
}

void Host::SynchronizeAll()
{
	for( auto i = this->zClients.begin(); i != this->zClients.end(); ++i )
	{
		this->zSynchronizer->SendUpdatesTo(i->second);
	}

	this->zSynchronizer->ClearAll();
}

// TODO: Create GameMode Here
void Host::Restart( const std::string& gameMode, const std::string& map )
{
// 	// Update
// 	this->zGameMode = gameMode;
// 	this->zMapName = map;
// 
// 	if ( this->zGame )
// 	{
// 		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, "Server Restarting");
// 		// Fake Disconnects
// 		for( auto i = this->zClients.begin(); i != this->zClients.end(); ++i )
// 		{
// 			i->second->Send(msg);
// 			PlayerDisconnectedEvent PDE;
// 			PDE.clientData = i->second;
// 			zGame->OnEvent(&PDE);
// 		}
// 
// 		// Delete Game
// 		this->RemoveObserver(zGame);
// 		delete zGame;
// 		FreePhysics();
// 	}
// 
// 	if ( this->zSynchronizer ) 
// 		this->zSynchronizer->ClearAll();
// 	if ( !this->zSynchronizer ) 
// 		this->zSynchronizer = new ActorSynchronizer();
// 
// 	// Start New
// 	PhysicsInit();
// 	this->zGame = new Game(GetPhysics(), this->zSynchronizer, gameMode, map);
// 	this->AddObserver(this->zGame);
// 
// 	// Fake Connects
// 	for( auto i = this->zClients.begin(); i != this->zClients.end(); ++i )
// 	{
// 		PlayerConnectedEvent PCE;
// 		PCE.clientData = i->second;
// 		zGame->OnEvent(&PCE);
// 	}

	this->zGame->RestartGame();
	this->zRestartRequested = false;


}