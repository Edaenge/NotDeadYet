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
	this->zTimeOut = 15.0f;
	this->zPingMessageInterval = 5.0f;
	this->zUpdateDelay = 0.05f;
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();

	this->Close();
	this->WaitUntillDone();

	this->zServerListener->Close();
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

	static float waitTimer = 0.0f;
	
	while(this->stayAlive)
	{
		waitTimer += Update();

		//Checks if ServerListener is still working
		if(!this->zServerListener->IsAlive())
		{
			MaloW::Debug("Server Listener has died.");
		}

		HandleNewConnections();
		ReadMessages();
		HandleRecivedMessages();
		PingClients();
		UpdatePl();
		
		if(waitTimer >= this->zUpdateDelay)
		{
			SendPlayerUpdates();
			waitTimer = 0.0f;
		}

		Sleep(5);
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
	if(!HasPlayers())
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

	this->zClients[pos]->zClient->sendData(message);
}

void Host::SendPlayerUpdates()
{
	if(!HasPlayers())
		return;

	std::vector<std::string> playerData;
	std::string mess = "";

	//Fetch player data
	std::vector<PlayerActor*>::iterator it_Player;
	for (it_Player = this->zPlayers.begin(); it_Player < this->zPlayers.end(); it_Player++)
	{
		Vector3 pos = (*it_Player)->GetPosition();
		Vector3 dir = (*it_Player)->GetDirection();
		Vector4 rot = (*it_Player)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_PLAYER, (*it_Player)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_FRAME_TIME, (*it_Player)->GetFrameTime());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it_Player)->GetState());
		//mess += this->zMessageConverter.Convert(MESSAGE_TYPE_DIRECTION, dir.x, dir.y, dir.z);

		playerData.push_back(mess);
	}

	//Send Data to clients
	std::vector<ClientData*>::iterator it_Client;
	std::vector<std::string>::iterator it_Message;

	for (it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (it_Message = playerData.begin(); it_Message < playerData.end(); it_Message++)
		{
			(*it_Client)->zClient->sendData((*it_Message));
		}
	}


}

bool Host::HasPlayers() const
{
	return !this->zClients.empty();
}

void Host::ReadMessages()
{
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

		if(pe)
		{
			MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe);
			if(np)
				this->zMessages.push_back(np);
			else
				SAFE_DELETE(pe);
		}
		
	}
}

void Host::HandleRecivedMessages()
{
	
	if(this->zMessages.empty())
		return;

	int p_index;
	int c_index;

	for (auto it = this->zMessages.begin(); it < this->zMessages.end(); it++)
	{
		//If null, skip and continue
		if(!(*it))
			continue;
		
		std::vector<std::string> msgArray;
		msgArray = this->zMessageConverter.SplitMessage((*it)->getMessage()); 

		//If empty, skip and continue
		if(msgArray.empty())
			continue;

		char key[512];
		sscanf(msgArray[0].c_str(), "%s ", key);
		
		c_index = SearchForClient((*it)->getID());
		p_index = SearchForPlayer((*it)->getID());

		//Handles upadtes from client.
		if(strcmp(key, CLIENT_DATA.c_str()) == 0 && (p_index != -1))
		{
			HandlePlayerUpdate(this->zPlayers[p_index], this->zClients[c_index], msgArray);
		}
		//Handles key presses from client.
		else if(strcmp(key, KEY_DOWN.c_str()) == 0 && (p_index != -1))
		{
			HandleKeyPress(this->zPlayers[p_index], msgArray[0]);
		}
		//Handles key releases from client.
		else if(strcmp(key, KEY_UP.c_str()) == 0 && (p_index != -1))
		{
			HandleKeyRelease(this->zPlayers[p_index], msgArray[0]);
		}
		//Handles Pings from client.
		else if(strcmp(key, PING.c_str()) == 0 && (c_index != -1))
		{
			HandlePingMsg(this->zClients[c_index]);
		}
		//Handles user data from client. Used when the player is new.
		else if(strcmp(key, USER_DATA.c_str()) == 0 && (c_index != -1))
		{
			CreateNewPlayer(this->zClients[c_index], msgArray);
		}
		//Handles if client disconnects.
		else if(strcmp(key, CONNECTION_CLOSED.c_str()) == 0)
		{
			KickClient((*it)->getID());
		}
		//Handles if not of the above.
		else
		{
			MaloW::Debug("Warning: The host cannot handle the message \""+(*it)->getMessage()+"\" in HandleRecivedMessages.");
		}

		SAFE_DELETE((*it));
	}
}

void Host::HandleKeyPress( PlayerActor* pl, const std::string& key )
{
	//Hard coded for test
	int keyz = this->zMessageConverter.ConvertStringToInt(KEY_DOWN, key);

	switch (keyz)
	{
	case KEY_FORWARD:
		pl->SetKeyState(KEY_FORWARD, true);
		break;
	case KEY_BACKWARD:
		pl->SetKeyState(KEY_BACKWARD, true);
		break;
	case KEY_LEFT:
		pl->SetKeyState(KEY_LEFT, true);
		break;
	case KEY_RIGHT:
		pl->SetKeyState(KEY_RIGHT, true);
		break;
	case KEY_SPRINT:
		pl->SetKeyState(KEY_SPRINT, true);
		break;
	case KEY_DUCK:
		pl->SetKeyState(KEY_DUCK, true);
		break;
	case KEY_JUMP:
		pl->SetKeyState(KEY_JUMP, true);
		break;
	default:
		break;
	}

}

void Host::HandleKeyRelease( PlayerActor* pl, const std::string& key )
{
	//Hard coded for test
	int keyz = this->zMessageConverter.ConvertStringToInt(KEY_UP, key);

	switch (keyz)
	{
	case KEY_FORWARD:
		pl->SetKeyState(KEY_FORWARD, false);
		break;
	case KEY_BACKWARD:
		pl->SetKeyState(KEY_BACKWARD, false);
		break;
	case KEY_LEFT:
		pl->SetKeyState(KEY_LEFT, false);
		break;
	case KEY_RIGHT:
		pl->SetKeyState(KEY_RIGHT, false);
		break;
	case KEY_SPRINT:
		pl->SetKeyState(KEY_SPRINT, false);
		break;
	case KEY_DUCK:
		pl->SetKeyState(KEY_DUCK, false);
		break;
	case KEY_JUMP:
		pl->SetKeyState(KEY_JUMP, false);
		break;
	default:
		break;
	}
}

void Host::HandlePingMsg( ClientData* cd )
{
	//Hard coded
	if(cd->zTotalPingTime > 60.0f)
		cd->ResetPingCounter();

	cd->zTotalPingTime += cd->zCurrentPingTime;
	cd->zNrOfPings++;

	cd->zPinged = false;
	cd->zCurrentPingTime = 0.0f;
}

void Host::HandlePlayerUpdate( PlayerActor* pl, ClientData* cd, const std::vector<std::string> &data )
{
	
	for(auto it = data.begin() + 1; it < data.end(); it++)
	{
		char key[1024];
		sscanf((*it).c_str(), "%s ", key);
		
		if(strcmp(key, DIRECTION.c_str()) == 0)
		{
			Vector3 dir = this->zMessageConverter.ConvertStringToVector(DIRECTION, (*it));
			pl->SetDirection(dir);
		}
		else if(strcmp(key, UP.c_str()) == 0)
		{
			Vector3 up = this->zMessageConverter.ConvertStringToVector(UP, (*it));
			pl->SetUpVector(up);
		}
		else if(strcmp(key, ROTATION.c_str()) == 0)
		{
			Vector4 rot = this->zMessageConverter.ConvertStringToQuaternion(ROTATION, (*it));
			pl->SetRotation(rot);
		}
		else if(strcmp(key, FRAME_TIME.c_str()) == 0)
		{
				float frameTime = this->zMessageConverter.ConvertStringToFloat(FRAME_TIME, (*it));
				pl->SetFrameTime(frameTime);
		}
		else
			MaloW::Debug("Unknown message in HandlePlayerUpdate.");
	}

	//Update Latency
	if( cd->zNrOfPings == 0)
		return;

	float latency = cd->zTotalPingTime / cd->zNrOfPings;
	pl->SetLatency(latency);
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
			if(cd->zCurrentPingTime < zPingMessageInterval)
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
			if(cd->zCurrentPingTime > zTimeOut)
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
	//Update Timer
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	return this->zDeltaTime;
}

void Host::UpdatePl()
{
	for (auto it = zPlayers.begin(); it < zPlayers.end(); it++)
	{
		(*it)->Update(this->zDeltaTime);
	}
	
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

void Host::CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data )
{
	std::string mess;
	std::string uModel;
	Vector3 uDir;
	Vector3 uUp;


	PlayerActor* pi = new PlayerActor(cd->zClient->getClientID());

	for (auto it_m = data.begin() + 1; it_m < data.end(); it_m++)
	{
		char key[512];
		sscanf((*it_m).c_str(), "%s ", key);

		if(strcmp(key, MESH_MODEL.c_str()) == 0)
		{
			uModel = this->zMessageConverter.ConvertStringToSubstring(MESH_MODEL, (*it_m));
		}
		else if(strcmp(key, DIRECTION.c_str()) == 0)
		{
			uDir = this->zMessageConverter.ConvertStringToVector(DIRECTION, (*it_m));
		}
		else if(strcmp(key, UP.c_str()) == 0)
		{
			uUp = this->zMessageConverter.ConvertStringToVector(UP, (*it_m));
		}
	}

	if(uModel != "")
		pi->SetPlayerModel(uModel);

	pi->SetUpVector(uUp);
	pi->SetDirection(uDir);
	//Debug Pos
	pi->SetPosition(Vector3(pi->GetID()*25,0,1)); 
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
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it)->GetState());
		//mess += this->zMessageConverter.Convert(MESSAGE_TYPE_DIRECTION, dir.x, dir.y, dir.z);

		temp.push_back(mess);

		if((*it)->GetID() == pi->GetID())
			newPlayerindex = count;

		count++;
	}

	//Send players to new player
	MaloW::ClientChannel* cc = cd->zClient;

	std::vector<std::string>::iterator sIt;
	for (sIt = temp.begin(); sIt < temp.end(); sIt++)
	{
		cc->sendData(*sIt);
	}

	//Send new player to players
	for (unsigned int i = 0; i < (unsigned int)this->zClients.size(); i++)
	{
		if(zClients[i]->zClient->getClientID() != cc->getClientID())
			this->zClients[i]->zClient->sendData(temp[newPlayerindex]);
	}

	
}