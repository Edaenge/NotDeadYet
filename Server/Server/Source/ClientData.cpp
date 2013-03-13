#include "ClientData.h"

#define MAX_STORED_LATENCY 5

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	this->zClient = cc;
	this->zLastRecivedPacketTime = 0.0f;
}

ClientData::~ClientData()
{
	if (this->zClient)
	{
		delete this->zClient;
		this->zClient = NULL;
	}
}

void ClientData::Kick()
{
	this->zClient->Disconnect();
}

float ClientData::GetAverageLatency()
{
	if( this->zLatency.empty() )
		return 0.0f;

	float total = 0.0f;
	for(auto it = this->zLatency.begin(); it < this->zLatency.end(); it++)
	{
		total += (*it); 
	}

	return total / this->zLatency.size();
}

void ClientData::AddLatency( const float time )
{
	if( zLatency.size() == MAX_STORED_LATENCY )
		this->zLatency.erase(this->zLatency.begin());

	this->zLatency.push_back(time);
}
