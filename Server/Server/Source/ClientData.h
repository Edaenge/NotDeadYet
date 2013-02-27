/*
Made by Edänge Simon 2013-01-24
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include <ClientChannel.h>
#include <Safe.h>
#include <vector>
#include <Packets\Packet.h>


class ClientData
{
	ClientData(MaloW::ClientChannel* cc);
	virtual ~ClientData();

	bool zPinged;
	float zCurrentPingDelay;
	float zCurrentPingTime;
	float zTotalPingTime;
	float zMaxPings;
	int zNrOfPings;
	bool zReady;

	MaloW::ClientChannel* zClient;
public:

	inline float GetCurrentPingTime() const {return this->zCurrentPingTime;}
	inline float GetTotalPingTime() const {return this->zTotalPingTime;}
	inline float GetCurrentPingDelayTime() const {return this->zCurrentPingDelay;}
	inline int GetNrOfPings() const {return this->zNrOfPings;}
	inline bool GetReady(){ return this->zReady; }
	
	inline void SetReady(bool ready){this->zReady = ready; }
	inline void SetPinged(const bool pinged) {this->zPinged = pinged;}
	inline void SetCurrentPingTime(float const cpt) {this->zCurrentPingTime = cpt;}
	inline void SetCurrentPingDelayTime(float const cpdt) {this->zCurrentPingDelay = cpdt;}
	inline bool HasBeenPinged() const {return this->zPinged;}
	inline void IncPingTime(float dt) {this->zCurrentPingTime += dt;}
	inline void IncPingDelayTimer(float dt) {this->zCurrentPingDelay += dt;}
	inline void ResetPingCounter() {this->zPinged = false; this->zTotalPingTime = 0.0f; this->zNrOfPings = 0;}

	/*! Sends a message to the client.*/
	inline void Send(const std::string& msg)
	{
		if ( zClient ) zClient->TrySend(msg);
	}

	inline void Send( const Packet& packet )
	{
		if ( zClient )
		{
			std::stringstream ss;
			
			// Notify Data Type
			ss << "PACKET";

			// Packet
			std::string typeName = typeid(packet).name();
			typeName.erase( typeName.begin(), typeName.begin() + 6 );

			// Write Packet Type Name
			unsigned int typeNameSize = typeName.length();
			ss.write( reinterpret_cast<const char*>(&typeNameSize), sizeof(unsigned int) );
			ss.write( &typeName[0], typeNameSize );

			// Write Packet Data
			if ( !packet.Serialize(ss) ) 
				throw("Failed Packet Serialization!");			

			// Send
			Send(ss.str());
		}
	}

	/*! Handle the ping from client.*/
	void HandlePingMsg();
	/*! Updates the latency of this client.*/
	bool CalculateLatency(float& latencyOut);
	/*! kicks the client.*/
	void Kick();

	friend class Host;
};