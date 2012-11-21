#include "Client.h"

Client::Client()
{
	this->zIP = "";
	this->zPort = 0;
}
int Client::Connect(std::string ip, int port)
{
	this->zIP = ip;
	this->zPort = port;
}
Client::~Client()
{

}
void Client::Life()
{
	while(this->stayAlive)
	{

	}
}
