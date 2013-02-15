#pragma once

#include "Packet.h"
#include <Vector.h>
#include <map>


class ServerFramePacket : public Packet
{
public:
	std::map< unsigned int, Vector3 > newPositions;
	std::map< unsigned int, Vector4 > newRotations;
	std::map< unsigned int, Vector3 > newScales;
	std::map< unsigned int, unsigned int> newStates;

	virtual bool Serialize( std::ostream& ss ) const
	{
		size_t num = newPositions.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = newPositions.begin(); i != newPositions.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}
		
		num = newRotations.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = newRotations.begin(); i != newRotations.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}
		
		num = newScales.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = newScales.begin(); i != newScales.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(i->second));
		}

		num = newStates.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = newStates.begin(); i != newStates.end(); ++i )
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
		for( unsigned int i = 0; i != num; ++i )
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			newPositions[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; ++i )
		{
			Vector4 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector4));
			newRotations[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; ++i )
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			newScales[key] = value;
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; ++i )
		{
			unsigned int value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(unsigned int));
			newStates[key] = value;
		}

		return true;
	}
};