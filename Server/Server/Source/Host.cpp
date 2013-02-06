#include "Host.h"
#include "ClientServerMessages.h"
#include "ClientConnectedEvent.h"
#include "ClientDisconnectedEvent.h"

// 50 updates per sec
static const float UPDATE_DELAY = 0.020f;

Host::Host()
{
	MaloW::ClearDebug();
	Messages::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): Server: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): Server: vld.h included.");
#endif

	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zMinClients = 0;

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
	this->zTimeOut = 15.0f;
	this->zPingMessageInterval = 5.0f;
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();
	
	this->Close();
	this->WaitUntillDone();

	this->zServerListener->Close();
	SAFE_DELETE(this->zServerListener);
}

void Host::Life()
{
	if (Messages::FileWrite())
		Messages::Debug("Host Process Started");

	this->zServerListener->Start();

	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	
	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);

	static float waitTimer = 0.0f;
	
	this->zGameStarted = false;
	while(this->stayAlive)
	{
		Update();


		ReadMessages();
		if(this->zGameStarted)
		{
			//PingClients();
		}
		else
		{
			/*int clientsReady = 0;
			for(int i = 0; i < this->zClients.size(); i++)
			{
				if(this->zClients.at(i)->GetReady()) clientsReady++;
			}
			if(clientsReady >= this->zClients.size() && this->zClients.size())
			{
				this->zGameStarted = true;
				this->SendStartMessage();
			}*/
			/*if (this->zClients.size() >= this->zMinClients)
			{
				this->zGameStarted = true;
				this->SendStartMessage();
			}*/
		}
	
		Sleep(5);
	}
}

const char* Host::InitHost(const unsigned int &port, const unsigned int &maxClients,  const std::string& gameModeName, const std::string& mapName)
{
	//Todo add map + game mode to parameters
	std::string path = "3x3.map";
	GameMode* gameMode = NULL;

	if (gameModeName.find("FFA") == 0 )
	{
		gameMode = new GameModeFFA();
	}
	else
	{
		gameMode = new GameModeFFA();
	}

	ActorSynchronizer* actorSync = new ActorSynchronizer();
	this->zGame = new Game(actorSync, gameMode, mapName + ".map");
	this->AddObserver(this->zGame);

	try
	{
		if ( zServerListener ) delete zServerListener;
		zServerListener = new ServerListener(this, port);
		zServerListener->Start();
	}
	catch(const char *str)
	{
		return str;
	}

	return 0;
}

void Host::SendToAllClients(const std::string& message, bool sendIM /*= false*/)
{
	if(!HasClients())
		return;
}

void Host::SendToClient(int clientID, const std::string& message, bool sendIM /*= false*/)
{
	
}

void Host::SendToClient(ClientData* cd, const std::string& message, bool sendIM /*= false*/)
{
}

bool Host::HasClients() const
{
	return false;
}

void Host::ReadMessages()
{
	
	static unsigned int MAX_MESSAGES_TO_READ = 10;
	unsigned int nrOfMessages = this->GetEventQueueSize();

	// No new messages
	if(nrOfMessages == 0)
		return;

	nrOfMessages = min(nrOfMessages, MAX_MESSAGES_TO_READ);
	MaloW::ProcessEvent* pe;

	for (unsigned int i = 0; i < nrOfMessages; i++)
	{
		pe = PeekEvent();

		if ( MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe) )
		{
			HandleReceivedMessage(dynamic_cast<MaloW::ClientChannel*>(np->GetChannel()), np->GetMessage());
		}
		else if ( ClientConnectedEvent* CCE = dynamic_cast<ClientConnectedEvent*>(pe) )
		{
			// A Client Connected
			HandleNewConnection(CCE->GetClientChannel());
		}
		else if ( ClientDisconnectedEvent* CDE = dynamic_cast<ClientDisconnectedEvent*>(pe) )
		{
			HandleDisconnect(CDE->GetClientChannel());
		}

		// Unhandled Message
		SAFE_DELETE(pe);
	}
}

void Host::HandleReceivedMessage( MaloW::ClientChannel* cc, const std::string &message )
{
	
	std::vector<std::string> msgArray;
	msgArray = this->zMessageConverter.SplitMessage(message); 
	ClientData* cd = this->_clients.at(cc);
	
	// Empty Array
	if ( msgArray.size() == 0 ) return;

	//Handles updates from client.
	if(msgArray[0].find(M_CLIENT_DATA.c_str()) == 0)
	{
		this->HandleClientUpdate(msgArray, cd);
	}
	//Handles key presses from client.
	else if(msgArray[0].find(M_KEY_DOWN.c_str()))
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
		/*this->zClients[c_index]->HandlePingMsg();*/

	}
	//Handles ready from client.
	else if(msgArray[0].find(M_READY_PLAYER.c_str()))
	{
		PlayerReadyEvent e;

		e.clientData = cd;
		NotifyObservers(&e);
	}
	//Handle Item usage in Inventory
	else if(msgArray[0].find(M_ITEM_USE.c_str()))
	{
		PlayerUseItemEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_USE, msgArray[0]);

		e.clientData = cd;
		e.itemID = _itemID;
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

		NotifyObservers(&e);
	}
	//Handles Equipped Weapon usage
	else if(msgArray[0].find(M_WEAPON_USE.c_str()))
	{
		PlayerUseEquippedWeaponEvent e;
		int _itemID = this->zMessageConverter.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);

		e.clientData = cd;
		e.itemID = _itemID;
		NotifyObservers(&e);
	}
	//Handles Pickup Object Requests from Client
	else if(msgArray[0].find(M_PICKUP_ITEM.c_str()))
	{
		PlayerPickupObjectEvent e;
		int _objID = this->zMessageConverter.ConvertStringToInt(M_PICKUP_ITEM, msgArray[0]);

		e.objID = _objID;
		NotifyObservers(&e);
	}
	//Handles loot item request
	else if(msgArray[0].find(M_LOOT_ITEM.c_str()))
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
	//Handles user data from client. Used when the player is new.
	else if(msgArray[0].find(M_USER_DATA.c_str()) == 0)
	{
		HandleUserData(msgArray, cd);
	}
	//Handles if client disconnects.
	else if(msgArray[0].find(M_CONNECTION_CLOSED.c_str()) == 0)
	{
		//this->KickClient(ID);
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
	SendToAllClients(mess);
}

//void Host::PingClients()
//{
//	if(!HasClients())
//		return;
//
//	ClientData* cd; 
//	MaloW::ClientChannel* ch;
//
//	for(unsigned int i = 0; i < (unsigned int)zClients.size(); i++)
//	{
//		cd = zClients.at(i);
//		ch = cd->GetClient();
//
//		//If client has not been pinged.
//		if(!cd->HasBeenPinged())
//		{
//			//If it was x sec ago we sent a ping, don't send a ping.
//			if(cd->GetCurrentPingTime() < zPingMessageInterval)
//				cd->IncPingTime(zDeltaTime);
//
//			//else send ping.
//			else
//			{
//				cd->SetCurrentPingTime(0.0f);
//				ch->Send(this->zMessageConverter.Convert(MESSAGE_TYPE_PING));
//				cd->SetPinged(true);
//			}
//		}
//		//If he have sent a ping.
//		else
//		{
//			//If we sent a ping x sec ago, drop the client.
//			if(cd->GetCurrentPingTime() > zTimeOut)
//			{
//				//cd->Kick(ch->GetClientID());
//			}
//			else
//				cd->IncPingTime(zDeltaTime);
//
//		}
//	}
//}

float Host::Update()
{
	//Update Timer
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	//Update the objects in the world.
	//this->zGameMode->Update(zDeltaTime);

	return this->zDeltaTime;
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::HandleDisconnect( MaloW::ClientChannel* channel )
{
	std::map<MaloW::ClientChannel*, ClientData*>::iterator it;

	it = _clients.find(channel);
	SAFE_DELETE( _clients.at(channel));
	SAFE_DELETE(channel);
	
	_clients.erase(it);
}

void Host::HandleNewConnection( MaloW::ClientChannel* CC )
{
	ClientData* cd = new ClientData(CC);
	PlayerConnectedEvent e;
	_clients[CC] = cd;
	
	e.clientData = cd;
	NotifyObservers(&e);
}

void Host::HandleClientUpdate( const std::vector<std::string> msgArray, ClientData* cd)
{
	Vector3 dir;
	Vector3 up;
	Vector4 rot;
	float ft = 0.0f;

	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		char key[512];
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			dir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it));
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			up = this->zMessageConverter.ConvertStringToVector(M_UP, (*it));
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rot = this->zMessageConverter.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			ft = this->zMessageConverter.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else
			MaloW::Debug("Unknown message in HandleClientUpdate.");
	}

	ClientDataEvent e;
	e.direction = dir;
	e.rotation = rot;
	e.upVector = up;
	e.frameTime = ft;
	e.clientData = cd;

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
	long _objID = -1;
	int _type = -1;
	long _itemID = -1;

	if (msgArray.size() > 2)
	{
		
		_objID = this->zMessageConverter.ConvertStringToInt(M_LOOT_ITEM, msgArray[0]);
		_itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_ID, msgArray[1]);
		_type = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);

		e.clientData = cd;
		e.itemID = _itemID;
		e.objID = _objID;
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
	std::string uModel;
	Vector3 uDir;
	Vector3 uUp;

	for (auto it_m = msgArray.begin() + 1; it_m < msgArray.end(); it_m++)
	{
		char key[512];
		sscanf_s((*it_m).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			uModel = this->zMessageConverter.ConvertStringToSubstring(M_MESH_MODEL, (*it_m));
		}
		else if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			uDir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it_m));
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			uUp = this->zMessageConverter.ConvertStringToVector(M_UP, (*it_m));
		}
	}

	e.upVector = uUp;
	e.direction = uDir;
	e.playerModel = uModel;
	e.clientData = cd;
	NotifyObservers(&e);
}

void Host::Message(MaloW::ClientChannel* cc, std::string msg)
{

}

