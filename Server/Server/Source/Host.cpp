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

const char* Host::InitHost(const unsigned int &port, const unsigned int &maxClients)
{
	//Todo add map + game mode to parameters
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

void Host::HandleNewConnection( MaloW::ClientChannel* CC )
{

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
	/*
	static unsigned int MAX_MESSAGES_TO_READ = 10;
	unsigned int nrOfMessages = this->GetEventQueueSize();

	//No new messsages
	if(nrOfMessages == 0)
		return;

	nrOfMessages = min(nrOfMessages, MAX_MESSAGES_TO_READ);
	MaloW::ProcessEvent* pe;

	for (unsigned int i = 0; i < nrOfMessages; i++)
	{
		pe = PeekEvent();

		if ( MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe) )
		{
			HandleReceivedMessage(np->getID(), np->getMessage());
		}
		else if ( ClientConnectedEvent* CCE = dynamic_cast<ClientConnectedEvent*>(pe) )
		{
			// A Client Connected
			HandleNewConnection(CCE->GetClientChannel());
		}
		else if ( ClientDisconnectedEvent* CDE = dynamic_cast<ClientDisconnectedEvent*>(pe) )
		{
			HandleDisconnect( CDE->GetClientChannel() );
		}

		// Unhandled Message
		SAFE_DELETE(pe);
	}*/
}

void Host::HandleReceivedMessage( const unsigned int &ID, const std::string &message )
{
	/*
	std::vector<std::string> msgArray;
	msgArray = this->zMessageConverter.SplitMessage(message); 
	
	// Empty Array
	if ( msgArray.size() == 0 ) return;

	int c_index = SearchForClient(ID);
	PlayerActor* p_actor = dynamic_cast<PlayerActor*>(this->zGameMode->GetActorHandlerPtr()->GetActor(ID, ACTOR_TYPE_PLAYER));

	//Handles updates from client.
	if(msgArray[0].find(M_CLIENT_DATA.c_str()) == 0 && (p_actor))
	{
		this->HandlePlayerUpdate(p_actor, this->zClients[c_index], msgArray);
	}
	//Handles key presses from client.
	else if(msgArray[0].find(M_KEY_DOWN.c_str()) == 0 && (p_actor))
	{
		int key = this->zMessageConverter.ConvertStringToInt(M_KEY_DOWN, msgArray[0]);

		this->zGameMode->HandleKeyPress(ID, key);
		//this->HandleKeyPress(p_actor, msgArray[0]);
	}
	//Handles key releases from client.
	else if(msgArray[0].find(M_KEY_UP.c_str()) == 0 && (p_actor))
	{
		int key = this->zMessageConverter.ConvertStringToInt(M_KEY_UP, msgArray[0]);

		this->zGameMode->HandleKeyRelease(ID, key);
		//this->HandleKeyRelease(p_actor, msgArray[0]);
	}
	//Handles Pings from client.
	else if(msgArray[0].find(M_PING.c_str()) == 0 && (c_index != -1))
	{
		this->zClients[c_index]->HandlePingMsg();
	}
	//Handles ready from client.
	else if(msgArray[0].find(M_READY_PLAYER.c_str()) == 0 && (c_index != -1))
	{
		this->zClients[c_index]->SetReady(true);
	}
	//Handles Ack Messages
	else if(msgArray[0].find(M_ACKNOWLEDGE_MESSAGE.c_str()) == 0 && (c_index != -1))
	{
		int m_id = this->zMessageConverter.ConvertStringToInt(M_ACKNOWLEDGE_MESSAGE, msgArray[0]);
		this->zClients[c_index]->RemoveIM(m_id);
	}
	//Handle Item usage in Inventory
	else if(msgArray[0].find(M_ITEM_USE.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
		this->HandleItemUse(p_actor, objID);
	}
	//Handle UnEquip Item in Equipment
	else if(msgArray[0].find(M_UNEQUIP_ITEM.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
		int eq_Slot = -1;
		if (msgArray.size() > 1)
		{
			eq_Slot = this->zMessageConverter.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
		}
		this->HandleUnEquipItem(p_actor, objID, eq_Slot);
	}
	//Handles Equipped Weapon usage
	else if(msgArray[0].find(M_WEAPON_USE.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
		this->HandleWeaponUse(p_actor, objID);
	}
	//Handles Pickup Object Requests from Client
	else if(msgArray[0].find(M_PICKUP_ITEM.c_str()) == 0 && (p_actor))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_PICKUP_ITEM, msgArray[0]);
		this->HandlePickupItem(p_actor, objID);
	}
	else if(msgArray[0].find(M_LOOT_ITEM.c_str()) == 0 && (p_actor))
	{
		long objID = this->zMessageConverter.ConvertStringToInt(M_LOOT_ITEM, msgArray[0]);
		int type = -1;
		long itemID = -1;
		if (msgArray.size() > 2)
		{
			itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_ID, msgArray[1]);
			type = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);

			if (this->HandleLootItem(p_actor, objID, itemID, type))
			{
				this->SendRemoveDeadPlayerItem(p_actor->GetID(), objID, itemID, type);
			}

		}
		else
		{
			MaloW::Debug("Msg array size is to short size: " + MaloW::convertNrToString((float)msgArray.size()) + " Expected size 3");
		}
	}
	//Handles Drop Item Requests from Client
	else if(msgArray[0].find(M_DROP_ITEM.c_str()) == 0 && (p_actor))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_DROP_ITEM, msgArray[0]);
		this->HandleDropItem(p_actor, objID);
	}
	//Handles user data from client. Used when the player is new.
	else if(msgArray[0].find(M_USER_DATA.c_str()) == 0 && (c_index != -1))
	{
		this->CreateNewPlayer(this->zClients[c_index], msgArray);
	}
	//Handles if client disconnects.
	else if(msgArray[0].find(M_CONNECTION_CLOSED.c_str()) == 0)
	{
		this->KickClient(ID);
	}
	//Handles if not of the above.
	else
	{
		MaloW::Debug("Warning: The host cannot handle the message \"" + message + "\" in HandleReceivedMessages.");
	}
	*/
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
	//KickClient(channel->GetClientID());
}

void Host::Message(MaloW::ClientChannel* cc, std::string msg)
{

}
