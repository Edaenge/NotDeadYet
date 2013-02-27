#include "ClientData.h"

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	this->zClient = cc;
	this->zPinged = false;
	this->zCurrentPingTime = 0.0f;
	this->zTotalPingTime = 0.0f;
	this->zNrOfPings = 0;
	this->zMaxPings = 2.0f;
	this->zCurrentPingDelay = 0.0f;
	zReady = false;
}

ClientData::~ClientData()
{
	SAFE_DELETE(this->zClient);
}

void ClientData::HandlePingMsg()
{
	if(zNrOfPings > zMaxPings)
		ResetPingCounter();

	zTotalPingTime += zCurrentPingTime;
	zNrOfPings++;

	zPinged = false;
	zCurrentPingTime = 0.0f;
}

bool ClientData::CalculateLatency( float& latencyOut )
{
	if( zNrOfPings == 0)
		return false;

	latencyOut = zTotalPingTime / zNrOfPings;

	return true;
}

void ClientData::Kick()
{
	this->zClient->Disconnect();
}