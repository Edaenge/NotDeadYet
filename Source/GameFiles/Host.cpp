#include "Host.h"

Host::Host()
{
	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zClients = new MaloW::Array<ClientData *>(); 
	this->zPlayers = new MaloW::Array<PlayerInfo *>();

	this->zStartime = 0.0f;
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
	}

	std::string message = "";

	MaloW::ClientChannel* client = cce->GetClientChannel();
	
	//Here the server should send player data to
	//all clients

	
	client->setNotifier(this);
	this->zPlayers->add(new PlayerInfo(client->getClientID()));
	this->zClients->add(new ClientData(client));

	SAFE_DELETE(pe);
}

void Host::SendToAllClients( std::string message )
{
	if(this->zClients->isEmpty())
		return;

	for(int i = 0; i < zClients->size(); i++)
	{
		zClients->get(i)->zClient->sendData(message);
	}
}

void Host::SendToClient( int clientID, std::string message )
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;

	this->zClients->get(pos)->zClient->sendData(message);
}

inline bool Host::HasPlayers() const
{
	return this->zClients->isEmpty();
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

	//Implement here
	std::vector<std::string> msgArray;
	msgArray = this->zMessageConverter.SplitMessage(np->getMessage()); 

	if(!msgArray.empty())
	{
		if(strcmp(msgArray[0].c_str(), PING.c_str()) == 0)
		{
			int index = SearchForClient(np->getID());

			ClientData* cd;
			cd = zClients->get(index);

			cd->zPinged = false;
			cd->zCurrentPingTime = 0.0f;
		}
	}

}

inline int Host::GetPort() const
{
	return this->zPort;
}

int Host::SearchForClient( int ID )
{

	if(!HasPlayers())
		return -1;

	for (int i = 0; i < 0; i++)
	{
		if(this->zClients->get(i)->zClient->getClientID() == ID)
		{
			return i;
		}
	}

	return -1;
}

inline void Host::BroadCastServerShutdown()
{
	std::string mess = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_SHUTDOWN);
	SendToAllClients(mess);
}

void Host::PingClients()
{
	if(!HasPlayers())
		return;

	ClientData* cd; 

	for(int i = 0; i < zClients->size(); i++)
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
				cd->zClient->sendData(PING);
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

bool Host::KickClient( int ID, bool sendAMessage /*= false*/, std::string reason /*= ""*/ )
{
	int index = SearchForClient(ID);

	if(index == -1)
	{
		MaloW::Debug("Can't find client to kick in KickClient in Host.cpp.");
		return false;
	}

	if(sendAMessage)
	{
		std::string mess;

		mess = zMessageConverter.Convert(MESSAGE_TYPE_KICKED);
		mess += reason;

		this->zClients->get(index)->zClient->sendData(mess);
	}


	return this->zClients->remove(index);
}

inline bool Host::IsAlive() const
{
	return this->stayAlive;
}



