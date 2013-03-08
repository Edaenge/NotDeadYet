#pragma once

#include "Packet.h"
#include <Vector.h>
#include <map>
#include "ClientServerMessages.h"

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
		size_t num = actorModel.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorModel.begin(); i != actorModel.end(); ++i )
		{
			unsigned int size = i->second.length();
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(unsigned int));
			ss.write(reinterpret_cast<const char*>(&size), sizeof(unsigned int));
			ss.write(&i->second[0], i->second.length());
			//Messages::Debug("Model");
			//Messages::Debug(MaloW::convertNrToString(i->first) + ": " + i->second);
		}

		num = actorPosition.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for (auto i = actorPosition.begin(); i != actorPosition.end(); i++)
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(unsigned int));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(Vector3));
			//Messages::Debug("Position");
			//Messages::Debug(MaloW::convertNrToString(i->first) + ": " +MaloW::convertNrToString(i->second.x) + "," + MaloW::convertNrToString(i->second.y) + "," +MaloW::convertNrToString(i->second.z));
		}

		num = actorRotation.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorRotation.begin(); i != actorRotation.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(unsigned int));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(Vector4));
			//Messages::Debug("Rotation");
			//Messages::Debug(MaloW::convertNrToString(i->first) + ": " +MaloW::convertNrToString(i->second.x) + "," + MaloW::convertNrToString(i->second.y) + "," +MaloW::convertNrToString(i->second.z) + "," +MaloW::convertNrToString(i->second.w));
		}

		num = actorScale.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorScale.begin(); i != actorScale.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(unsigned int));
			ss.write(reinterpret_cast<const char*>(&i->second.x), sizeof(Vector3));
			//Messages::Debug("Scale");
			//Messages::Debug(MaloW::convertNrToString(i->first) + ": " +MaloW::convertNrToString(i->second.x) + ", " + MaloW::convertNrToString(i->second.y) + "," +MaloW::convertNrToString(i->second.z));
		}

		num = actorState.size();
		ss.write(reinterpret_cast<const char*>(&num), sizeof(size_t));
		for( auto i = actorState.begin(); i != actorState.end(); ++i )
		{
			ss.write(reinterpret_cast<const char*>(&i->first), sizeof(unsigned int));
			ss.write(reinterpret_cast<const char*>(&i->second), sizeof(unsigned int));
			//Messages::Debug("State");
			//Messages::Debug(MaloW::convertNrToString(i->first) + ": " +MaloW::convertNrToString(i->second));
		}
		return true;
	}

	virtual bool Deserialize( std::istream& ss )
	{
		size_t num = 0;
		unsigned int key;

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
			//Messages::Debug("Model");
			//Messages::Debug(MaloW::convertNrToString(key) + ": " + value);
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for (unsigned int i = 0; i != num; i++)
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			actorPosition[key] = value;
			//Messages::Debug("Position");
			//Messages::Debug(MaloW::convertNrToString(key) + ": " +MaloW::convertNrToString(value.x) + ", " + MaloW::convertNrToString(value.y) + "," +MaloW::convertNrToString(value.z));
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			Vector4 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector4));
			actorRotation[key] = value;
			//Messages::Debug("Rotation");
			//Messages::Debug(MaloW::convertNrToString(key) + ": " +MaloW::convertNrToString(value.x) + ", " + MaloW::convertNrToString(value.y) + "," +MaloW::convertNrToString(value.z) + "," +MaloW::convertNrToString(value.w));
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			Vector3 value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(Vector3));
			actorScale[key] = value;
			//Messages::Debug("Scale");
			//Messages::Debug(MaloW::convertNrToString(key) + ": " +MaloW::convertNrToString(value.x) + ", " + MaloW::convertNrToString(value.y) + "," +MaloW::convertNrToString(value.z));
		}

		ss.read(reinterpret_cast<char*>(&num), sizeof(size_t));
		for( unsigned int i = 0; i != num; i++)
		{
			unsigned int value;
			ss.read(reinterpret_cast<char*>(&key), sizeof(unsigned int));
			ss.read(reinterpret_cast<char*>(&value), sizeof(unsigned int));
			actorState[key] = value;
			//Messages::Debug("State");
			//Messages::Debug(MaloW::convertNrToString(key) + ": " +MaloW::convertNrToString(value));
		}
		return true;
	}
};