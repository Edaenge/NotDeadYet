#include "Host.h"

Host::Host()
{
	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zClients = new MaloW::Array<MaloW::ClientChannel *>(); 
}

Host::~Host()
{
	SAFE_DELETE(this->zServerListener);
	SAFE_DELETE(this->zClients);
}

void Host::Life()
{
	this->zServerListener->Start();

	while(this->stayAlive)
	{
		
	}
}

int Host::InitHost(int port, int maxClients)
{
	int code = 0;

	if(!this->zServerListener)
		this->zServerListener = new ServerListener();
	else
	{
		SAFE_DELETE(this->zServerListener);
		this->zServerListener;
	}
	
	code = this->zServerListener->InitListener(port);
	
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
	
	//Create an new player message
	//message += zMessageConverter.Convert() 

	zClients->add(client);

	SAFE_DELETE(pe);
}
