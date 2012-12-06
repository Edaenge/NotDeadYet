#include "GameFiles/ServerSide/Host.h"

Host::Host()
{
	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zClients = std::vector<ClientData *>(); 
	this->zPlayers = std::vector<PlayerActor *>();

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();

	this->Close();
	this->WaitUntillDone();

	this->zServerListener->Close();
	this->zServerListener->WaitUntillDone();
	SAFE_DELETE(this->zServerListener);

	for(auto x = zPlayers.begin(); x < zPlayers.end(); x++)
	{
		SAFE_DELETE(*x);
	}
	for(auto x = zClients.begin(); x < zClients.end(); x++)
	{
		SAFE_DELETE(*x);
	}
}

void Host::Life()
{
	this->zServerListener->Start();
	
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	
	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);
	
	while(this->stayAlive)
	{
		Update();

		//Checks if ServerListener is still working
		if(!this->zServerListener->IsAlive())
		{
			int code;

			this->zServerListener->Close();
			this->zServerListener->WaitUntillDone();

			SAFE_DELETE(this->zServerListener);
			this->zServerListener = new ServerListener();
			code = this->zServerListener->InitListener(this->zPort);

			if(code == 0)
				this->zServerListener->Start();

			else
			{
				MaloW::Debug("Failed to restart ServerListener. Error: " + MaloW::convertNrToString(code));
				this->Close();
			}
		}

		HandleNewConnections();
		HandleRecivedMessages();
		PingClients();
		
		Sleep(10);
	}
}

int Host::InitHost( int port, unsigned int maxClients )
{
	int code = 0;

	if(!this->zServerListener)
		this->zServerListener = new ServerListener();
	else
	{
		SAFE_DELETE(this->zServerListener);
		this->zServerListener = new ServerListener();
	}
	
	code = this->zServerListener->InitListener(port);

	this->zMaxClients = maxClients;
	this->zPort	= port;
	
	return code;
}

void Host::HandleNewConnections()
{
	MaloW::ProcessEvent* pe; 
	pe = this->zServerListener->PeekEvent();

	if(!pe)
		return;

	ClientConnectedEvent* cce = dynamic_cast<ClientConnectedEvent*>(pe);
	if(!cce)
	{
		SAFE_DELETE(pe);
		return;
	}

	MaloW::Debug("New Player Connected.");

	if((unsigned int)this->zClients.size() > zMaxClients)
	{
		std::string message;

		message = zMessageConverter.Convert(MESSAGE_TYPE_SERVER_FULL);

		cce->GetClientChannel()->sendData(message);
		SAFE_DELETE(pe);

		return;
	}

	MaloW::Debug("New Player Accepted.");

	std::string message = "";

	MaloW::ClientChannel* client = cce->GetClientChannel();

	client->setNotifier(this);
	this->zClients.push_back(new ClientData(client));

	message = this->zMessageConverter.Convert(MESSAGE_TYPE_SELF_ID, client->getClientID());
	client->sendData(message);
	client->Start();

	SAFE_DELETE(pe);
}

void Host::SendToAllClients( const std::string& message )
{
	if(this->zClients.empty())
		return;

	std::vector<ClientData*>::iterator it;

	for(it = zClients.begin(); it < zClients.end(); it++)
	{
		(*it)->zClient->sendData(message);
	}
}

void Host::SendToClient( int clientID, const std::string& message )
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;

	this->zClients.at(pos)->zClient->sendData(message);
}

bool Host::HasPlayers() const
{
	return !this->zClients.empty();
}

void Host::HandleRecivedMessages()
{
	//No new messsages
	if(this->GetEventQueueSize() == 0)
		return;

	//Fetch event
	MaloW::ProcessEvent* pe = PeekEvent();

	//If null, return
	if(!pe)
		return;

	MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe);

	if(!np)
	{
		SAFE_DELETE(pe);
		return;
	}

	std::vector<std::string> msgArray;
	msgArray = this->zMessageConverter.SplitMessage(np->getMessage()); 

	if(msgArray.empty())
		return;

	char key[512];
	sscanf(msgArray[0].c_str(), "%s ", key);

	if(strcmp(key, KEY_DOWN.c_str()) == 0)
	{
		HandleKeyPress(np->getID(), msgArray[0]);
	}
	else if(strcmp(key, KEY_UP.c_str()) == 0)
	{

	}
	else if(strcmp(key, PING.c_str()) == 0)
	{
		HandlePingMsg(np->getID());
	}
	else if(strcmp(key, USER_DATA.c_str()) == 0)
	{
		CreateNewPlayer(np->getID(), msgArray);
	}
	else if(strcmp(key, CONNECTION_CLOSED.c_str()) == 0)
	{
		KickClient(np->getID());
	}
	else
	{
		MaloW::Debug("Warning: The host cannot handle the message \""+np->getMessage()+"\" in HandleRecivedMessages.");
	}

}

void Host::HandleKeyPress( const int CLIENT_ID, const std::string& key )
{
	//Hard coded for test
	int keyz = this->zMessageConverter.ConvertStringToInt(KEY_DOWN, key);
	int index = SearchForPlayer(CLIENT_ID);

	PlayerActor* player = this->zPlayers.at(index);

	switch (keyz)
	{
	case KEY_FORWARD:
		player->SetKeyState(KEY_FORWARD, true);
		break;
	case KEY_BACKWARD:
		player->SetKeyState(KEY_BACKWARD, true);
		break;
	case KEY_LEFT:
		player->SetKeyState(KEY_LEFT, true);
		break;
	case KEY_RIGHT:
		player->SetKeyState(KEY_RIGHT, true);
		break;
	case KEY_SPRINT:
		player->SetKeyState(KEY_SPRINT, true);
		break;
	case KEY_DUCK:
		player->SetKeyState(KEY_DUCK, true);
		break;
	case KEY_JUMP:
		player->SetKeyState(KEY_JUMP, true);
		break;
	default:
		break;
	}

}

void Host::HandlePingMsg( const int CLIENT_ID )
{
	int index = SearchForClient(CLIENT_ID);

	ClientData* cd;
	cd = this->zClients.at(index);

	cd->zPinged = false;
	cd->zCurrentPingTime = 0.0f;
}

int Host::GetPort() const
{
	return this->zPort;
}

int Host::SearchForClient( const int ID ) const
{

	if(!HasPlayers())
		return -1;

	for (unsigned int i = 0; i < this->zClients.size(); i++)
	{
		if(this->zClients.at(i)->zClient->getClientID() == ID)
		{
			return i;
		}
	}

	return -1;
}

int Host::SearchForPlayer(const int ID) const
{
	if(!HasPlayers())
		return -1;

	for (unsigned int i = 0; i < this->zPlayers.size(); i++)
	{
		if(this->zPlayers.at(i)->GetID() == ID)
		{
			return i;
		}
	}

	return -1;
}

void Host::BroadCastServerShutdown()
{
	std::string mess = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_SHUTDOWN);
	SendToAllClients(mess);
}

void Host::PingClients()
{
	if(!HasPlayers())
		return;

	ClientData* cd; 

	for(unsigned int i = 0; i < (unsigned int)zClients.size(); i++)
	{
		cd = zClients.at(i);

		//If client has not been pinged.
		if(!cd->zPinged)
		{
			//If it was x sec ago we sent a ping, don't send a ping.
			if(cd->zCurrentPingTime < 5.0f) //hard coded
				cd->IncPingTime(zDeltaTime);

			//else send ping.
			else
			{
				cd->zCurrentPingTime = 0.0f;
				cd->zClient->sendData(this->zMessageConverter.Convert(MESSAGE_TYPE_PING));
				cd->zPinged = true;
			}
		}
		//If he have sent a ping.
		else
		{
			//If we sent a ping x sec ago, drop the client.
			if(cd->zCurrentPingTime > 10.0f)
			{
				KickClient(cd->zClient->getClientID());
			}
			else
				cd->IncPingTime(zDeltaTime);

		}
	}
}

float Host::Update()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	return this->zDeltaTime;
}

bool Host::KickClient( const int ID, bool sendAMessage /*= false*/, std::string reason /*= ""*/ )
{
	int index = SearchForClient(ID);
	int pIndex = SearchForPlayer(ID);

	ClientData* temp_c = zClients.at(index);
	PlayerActor* temp_p = zPlayers.at(pIndex);

	std::string mess;
	bool removed = false;

	if(index == -1)
	{
		MaloW::Debug("Can't find client to kick in KickClient in Host.cpp.");
		return false;
	}

	if(sendAMessage)
	{
		mess = this->zMessageConverter.Convert(MESSAGE_TYPE_KICKED, reason);

		temp_c->zClient->sendData(mess);
	}

	//create a remove player message.
	mess = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_PLAYER, ID);

	//remove the player


	if(index != -1 && pIndex != -1)
	{
		this->zClients.erase(zClients.begin() + index);
		this->zPlayers.erase(zPlayers.begin() + pIndex);

		SAFE_DELETE(temp_c);
		SAFE_DELETE(temp_p);

		removed = true;
		MaloW::Debug("Client"+MaloW::convertNrToString(ID)+" removed from server.");
	}

	//Notify clients
	this->SendToAllClients(mess);

	return removed;
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::CreateNewPlayer( const int ID, std::vector<std::string> &mesh)
{
	std::string uModel;
	std::string mess;
	PlayerActor* pi = new PlayerActor(ID);

	uModel = this->zMessageConverter.ConvertStringToSubstring(USER_DATA, mesh[0]);

	pi->SetPlayerModel(uModel);
	//Debug Pos
	pi->SetPosition(Vector3(ID*25,0,1)); 
	this->zPlayers.push_back(pi);

	//Create a new player message
	std::vector<std::string> temp;
	int newPlayerindex = 0;
	int count = 0;

	std::vector<PlayerActor*>::iterator it;
	for (it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector3 dir = (*it)->GetDirection();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetPlayerModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it)->GetState());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_DIRECTION, dir.x, dir.y, dir.z);

		temp.push_back(mess);

		if((*it)->GetID() == ID)
			newPlayerindex = count;

		count++;
	}

	//Send players to new player
	unsigned int clientIndex = SearchForClient(ID);
	MaloW::ClientChannel* cc = this->zClients.at(clientIndex)->zClient;

	std::vector<std::string>::iterator sIt;
	for (sIt = temp.begin(); sIt < temp.end(); sIt++)
	{
		cc->sendData(*sIt);
	}

	//Send new player to players
	for (unsigned int i = 0; i < (unsigned int)this->zClients.size(); i++)
	{
		if(i != clientIndex)
			this->zClients.at(i)->zClient->sendData(temp[newPlayerindex]);
	}

	
}





