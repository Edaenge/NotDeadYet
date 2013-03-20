#pragma once

#include "AnimationFileReader.h"
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
	std::map<unsigned int, std::string> newMesh;
	std::map<unsigned int, AnimationQueue> newAnimQueue;

	virtual ~ServerFramePacket() {}

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

		num = newAnimQueue.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for (auto i = newAnimQueue.begin(); i != newAnimQueue.end(); ++i)
		{
			unsigned int size = 0;
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));

			size_t vecSize = i->second.zAnimations.size();
			ss.write(reinterpret_cast<const char*>(&vecSize), sizeof(size_t));
			for(auto it = i->second.zAnimations.begin(); it != i->second.zAnimations.end(); it++)
			{
				size = (*it).length();
				ss.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));
				ss.write(&(*it)[0], (*it).length());
			}
			
			vecSize = i->second.zAnimationTimes.size();
			ss.write(reinterpret_cast<const char*>(&vecSize), sizeof(size_t));
			for(auto it = i->second.zAnimationTimes.begin(); it != i->second.zAnimationTimes.end(); it++)
			{
				ss.write(reinterpret_cast<const char*>(&(*it)), sizeof(float));
			}
		}

		num = newMesh.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = newMesh.begin(); i != newMesh.end(); ++i )
		{
			unsigned int size = i->second.length();
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(i->first));
			ss.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));
			ss.write(&i->second[0], i->second.length());

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

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for(unsigned int i = 0; i != num; i++)
		{
			std::string value;
			AnimationQueue queue;
			size_t vecSize = 0;
			unsigned int size = 0;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));

			ss.read(reinterpret_cast<char*>(&vecSize), sizeof(size_t));
			for (unsigned int i = 0; i != vecSize; i++)
			{
				ss.read(reinterpret_cast<char*>(&size), sizeof(unsigned int));
				value.resize(size);
				ss.read(&value[0], size);
				queue.zAnimations.push_back(value);
			}

			ss.read(reinterpret_cast<char*>(&vecSize), sizeof(size_t));
			for (unsigned int i = 0; i != vecSize; i++)
			{
				float fValue;
				ss.read(reinterpret_cast<char*>(&fValue), sizeof(float));
				queue.zAnimationTimes.push_back(fValue);
			}
			newAnimQueue[key] = queue;
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
			newMesh[key] = value;
		}

		return true;
	}

	inline virtual bool IsEmpty() const
	{
		unsigned int size = 0;
		size += newPositions.size();
		size += newRotations.size();
		size += newScales.size();
		size += newStates.size();
		size += newMesh.size();
		size += newAnimQueue.size();

		if( size == 0)
			return true;

		return false;
	}
};