#pragma once

#include "Packet.h"
#include <Vector.h>
#include <map>

class NewActorPacket : public Packet
{
public:
	std::map<unsigned int, Vector3> actorPosition;
	std::map<unsigned int, Vector4> actorRotation;
	std::map<unsigned int, Vector3> actorScale;
	std::map<unsigned int, std::string> actorModel;
	std::map<unsigned int, unsigned int> actorState;

	NewActorPacket() {};
	virtual ~NewActorPacket(){}

	virtual bool Serialize( std::ostream& ss ) const
	{
		size_t num = actorPosition.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for (auto i = actorPosition.begin(); i != actorPosition.end(); i++)
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}

		num = actorRotation.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorRotation.begin(); i != actorRotation.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}

		num = actorScale.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorScale.begin(); i != actorScale.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}

		num = actorModel.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorModel.begin(); i != actorModel.end(); ++i )
		{
			unsigned int size = i->second.length();
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));
			ss.write(&i->second[0], i->second.length());
		}

		num = actorState.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorState.begin(); i != actorState.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second), sizeof(i->second));
		}
		return true;
	}

	virtual bool Deserialize( std::istream& ss )
	{
		size_t num = 0;
		unsigned int key;

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for (unsigned int i = 0; i != num; i++)
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(size_t));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			actorPosition[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			Vector4 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector4));
			actorRotation[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			actorScale[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			std::string value;
			unsigned int size = 0;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&size), sizeof(unsigned int));
			value.resize(size);
			ss.read(&value[0], size);
			actorModel[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			unsigned int value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(unsigned int));
			actorState[key] = value;
		}
		return true;
	}
};