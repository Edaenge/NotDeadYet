#include "Host.h"

Host::Host()
{
	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zClients = new MaloW::Array<ClientData *>(); 
	this->zPlayers = new MaloW::Array<PlayerActor *>();

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();

	SAFE_DELETE(this->zServerListener);
	SAFE_DELETE(this->zClients);
	SAFE_DELETE(this->zPlayers);
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
		
	}
}

int Host::InitHost( int port, int maxClients )
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

	if(this->zClients->size() > zMaxClients)
	{
		std::string message;

		message = zMessageConverter.Convert(MESSAGE_TYPE_SERVER_FULL);

		cce->GetClientChannel()->sendData(message);
		SAFE_DELETE(pe);

		return;
	}

	std::string message = "";

	MaloW::ClientChannel* client = cce->GetClientChannel();

	client->setNotifier(this);
	this->zPlayers->add(new PlayerActor(client->getClientID()));
	this->zClients->add(new ClientData(client));

	message = this->zMessageConverter.Convert(MESSAGE_TYPE_SELF_ID, client->getClientID());
	client->sendData(message);
	client->Start();

	SAFE_DELETE(pe);
}

void Host::SendToAllClients( const std::string& message )
{
	if(this->zClients->isEmpty())
		return;

	for(int i = 0; i < zClients->size(); i++)
	{
		zClients->get(i)->zClient->sendData(message);
	}
}

void Host::SendToClient( int clientID, const std::string& message )
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;

	this->zClients->get(pos)->zClient->sendData(message);
}

bool Host::HasPlayers() const
{
	return !this->zClients->isEmpty();
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
	sscanf_s(msgArray[0].c_str(), "%s ", key);

	if(strcmp(key, KEY_DOWN.c_str()) == 0)
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

}

void Host::HandleKeyPress( const int CLIENT_ID, const std::string& key )
{
	//Hard coded for test
	std::string keyz = this->zMessageConverter.ConvertStringToSubstring(KEY_DOWN, key);
	int index = SearchForPlayer(CLIENT_ID);

	PlayerActor* player = this->zPlayers->get(index);

	if(keyz == "W")
	{
		
	}
	else if(keyz == "S")
	{

	}
	if(keyz == "A")
	{

	}
	if(keyz == "D")
	{
		
	}

}

void Host::HandlePingMsg( const int CLIENT_ID )
{
	int index = SearchForClient(CLIENT_ID);

	ClientData* cd;
	cd = this->zClients->get(index);

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

	for (unsigned int i = 0; i < (unsigned int)this->zClients->size(); i++)
	{
		if(this->zClients->get(i)->zClient->getClientID() == ID)
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

	for (unsigned int i = 0; i < (unsigned int)this->zPlayers->size(); i++)
	{
		if(this->zPlayers->get(i)->GetID() == ID)
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

	for(unsigned int i = 0; i < (unsigned int)zClients->size(); i++)
	{
		cd = zClients->get(i);

		//If client has not been pinged.
		if(!cd->zPinged)
		{
			//If it was x sec ago we sent a ping, don't send a ping.
			if(cd->zCurrentPingTime < 3.0f) //hard coded
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

		this->zClients->get(index)->zClient->sendData(mess);
	}

	//create a remove player message.
	mess = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_PLAYER, ID);

	//remove the player
	int pIndex = SearchForPlayer(ID);
	this->zPlayers->remove(index);

	if(pIndex == -1)
		removed = this->zClients->remove(pIndex);

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
	std::string uModel, mess;
	PlayerActor* pi = new PlayerActor(ID);

	uModel = this->zMessageConverter.ConvertStringToSubstring(USER_DATA, mesh[0]);

	pi->SetPlayerModel(uModel);
	this->zPlayers->add(pi);

	//Create a new player message
	PlayerActor* temp_PI;
	std::vector<std::string> temp;
	int newPlayerindex = 0;

	for (unsigned int i = 0; i < (unsigned int)this->zPlayers->size(); i++)
	{
		temp_PI = this->zPlayers->get(i);

		Vector3 pos = temp_PI->GetPosition();
		Vector3 scale = temp_PI->GetScale();
		Vector3 dir = temp_PI->GetDirection();
		Vector4 rot = temp_PI->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, temp_PI->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, temp_PI->GetPlayerModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, temp_PI->GetState());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_DIRECTION, dir.x, dir.y, dir.z);

		temp.push_back(mess);

		if(temp_PI->GetID() == ID)
			newPlayerindex = i;
	}

	//Send players to new player
	unsigned int clientIndex = SearchForClient(ID);
	MaloW::ClientChannel* cc = this->zClients->get(clientIndex)->zClient;

	std::vector<std::string>::iterator it;
	for (it = temp.begin(); it < temp.end(); it++)
	{
		cc->sendData(*it);
	}

	//Send new player to players
	for (unsigned int i = 0; i < (unsigned int)this->zClients->size(); i++)
	{
		if(i != clientIndex)
			this->zClients->get(i)->zClient->sendData(temp[newPlayerindex]);
	}

	
}





