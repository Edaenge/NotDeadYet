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

	bool zReady;
	float zLastRecivedPacketTime;
	std::vector<float> zLatency;

	MaloW::ClientChannel* zClient;
public:


	inline bool GetReady(){ return this->zReady; }
	/*! Returns the time of the last packet received from the client.*/
	inline float GetLastPacketTime() {return this->zLastRecivedPacketTime;}
	/*! Calculates and returns the average latency.*/
	float GetAverageLatency();

	inline void SetReady(bool ready){this->zReady = ready; }
	/*! Sets the time of the last packet received from the client.*/
	inline void SetLastPacketTime(const float packet) {this->zLastRecivedPacketTime;};
	/*! Adds the latency to the list.*/
	void AddLatency(const float time);
	/*! Sends a message to the client.*/
	inline void Send(const std::string& msg)
	{
		if ( zClient ) zClient->TrySend(msg);
	}
	/*! Sends a packet to the client.*/
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
	/*! kicks the client.*/
	void Kick();

	friend class Host;
};