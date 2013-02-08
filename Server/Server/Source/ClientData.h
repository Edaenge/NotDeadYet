/*
Made by Edänge Simon 2013-01-24
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include <ClientChannel.h>
#include <Safe.h>
#include <vector>


class ClientData
{
	ClientData(MaloW::ClientChannel* cc);
	virtual ~ClientData();

	bool zPinged;
	float zCurrentPingTime;
	float zTotalPingTime;
	float zMaxPingTime;
	int zNrOfPings;
	bool zReady;

	MaloW::ClientChannel* zClient;
public:

	inline float GetCurrentPingTime() const {return zCurrentPingTime;}
	inline float GetTotalPingTime() const {return zTotalPingTime;}
	inline int GetNrOfPings() const {return zNrOfPings;}
	inline bool GetReady(){ return zReady; }
	
	inline void SetReady(bool ready){ zReady = ready; }
	inline void SetPinged(const bool pinged) {zPinged = pinged;}
	inline void SetCurrentPingTime(float const cpt) {zCurrentPingTime = cpt;}
	inline bool HasBeenPinged() const {return zPinged;}
	inline void IncPingTime(float dt) {zCurrentPingTime += dt;}
	inline void ResetPingCounter() {zPinged = 0; zTotalPingTime = 0.0f;}

	/*! Sends a message to the client.*/
	inline void Send(const std::string& msg)
	{
		if ( zClient ) zClient->TrySend(msg);
	}

	/*! Handle the ping from client.*/
	void HandlePingMsg();
	/*! Updates the latency of this client.*/
	bool CalculateLatency(float& latencyOut);
	/*! kicks the client.*/
	void Kick();

	friend class Host;
};