#include "ClientData.h"

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	zClient = cc;
	zPinged = false;
	zCurrentPingTime = 0.0f;
	zTotalPingTime = 0.0f;
	zMaxPingTime = 0.0f;
	zNrOfPings = 0;
	zMaxPingTime = 10.0f;
	zReady = false;
}

ClientData::~ClientData()
{
	SAFE_DELETE(this->zClient);
}

void ClientData::HandlePingMsg()
{
	if(zTotalPingTime > zMaxPingTime)
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