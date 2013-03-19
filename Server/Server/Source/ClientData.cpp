#include "ClientData.h"

#define MAX_STORED_LATENCY 5

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	this->zClient = cc;
	this->zLastRecivedPacketTime = 0.0f;
}

ClientData::~ClientData()
{
	try
	{
		if (this->zClient)
		{
			delete this->zClient;
			this->zClient = NULL;
		}
	}
	catch(...)
	{

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

void ClientData::Send( const Packet& packet )
{
	if ( zClient && !packet.IsEmpty() )
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

void ClientData::Send(const std::string& msg)
{
	if (zClient) zClient->Send(msg);
}