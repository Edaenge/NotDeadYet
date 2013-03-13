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

	float zLastRecivedPacketTime;
	std::vector<float> zLatency;

	MaloW::ClientChannel* zClient;
public:

	/*! Returns the time of the last packet received from the client.*/
	inline float GetLastPacketTime() const {return this->zLastRecivedPacketTime;}

	/*! Calculates and returns the average latency.*/
	float GetAverageLatency();

	/*! Sets the time of the last packet received from the client.*/
	inline void SetLastPacketTime(const float& packet) { this->zLastRecivedPacketTime = packet; }

	/*! Adds the latency to the list.*/
	void AddLatency(const float time);

	/*! Sends a message to the client.*/
	void Send(const std::string& msg);

	/*! Sends a packet to the client.*/
	void Send(const Packet& packet);

	/*! kicks the client.*/
	void Kick();

	friend class Host;
};