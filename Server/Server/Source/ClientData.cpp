#include "ClientData.h"

#define MAX_STORED_LATENCY 10

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	this->zClient = cc;
	this->zLastRecivedPacketTime = 0.0f;

	zReady = false;
}

ClientData::~ClientData()
{
	SAFE_DELETE(this->zClient);
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
	for(auto it = zLatency.begin(); it < zLatency.end(); it++)
	{
		total += (*it); 
	}

	return total / zLatency.size();
}

void ClientData::AddLatency( const float time )
{
	if( zLatency.size() == MAX_STORED_LATENCY )
		this->zLatency.pop_back();

	this->zLatency.push_back(time);
}
