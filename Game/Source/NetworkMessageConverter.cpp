#include "NetworkMessageConverter.h"
#include <sstream>
#include <algorithm>

NetworkMessageConverter::NetworkMessageConverter()
{
}

NetworkMessageConverter::~NetworkMessageConverter()
{
}

std::string NetworkMessageConverter::Convert(const unsigned int ID, const Vector3& vec)
{
	std::stringstream ss;

	float x,y,z;
	x = vec.x;
	y = vec.y;
	z = vec.z;

	switch (ID)
	{
	case MESSAGE_TYPE_UP:
		ss << M_UP;
		break;
	case MESSAGE_TYPE_DIRECTION:
		ss << M_DIRECTION;
		break;
	case MESSAGE_TYPE_POSITION:
		ss << M_POSITION;
		break;
	case MESSAGE_TYPE_SCALE:
		ss << M_SCALE; 
		break;
	case MESSAGE_TYPE_SUN_DIRECTION:
		ss << M_SUN_DIRECTION;
		break;
	default:
		ss << "";
		break;
	}
	ss << " " << x << " " << y << " " << z <<"*";
	return ss.str();
}

std::string NetworkMessageConverter::Convert(const unsigned int ID, const Vector4& vec)
{
	std::stringstream ss;

	float x,y,z,w;
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;

	switch (ID)
	{
	case MESSAGE_TYPE_ROTATION:
		ss << M_ROTATION;
		break;
	default:
		return "";
		break;
	}
	ss << " " << x << " " << y << " "<<
		z << " " << w << "*";

	return ss.str();
}

std::string NetworkMessageConverter::Convert(const unsigned int ID, const std::string& word)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_MESH_MODEL:
		ss << M_MESH_MODEL;
		break;
	case MESSAGE_TYPE_KEY_DOWN:
		ss << M_KEY_DOWN;
		break;
	case MESSAGE_TYPE_KEY_UP:
		ss << M_KEY_UP;
		break;
	case MESSAGE_TYPE_PLAY_ANIMATION:
		ss << M_PLAY_ANIMATION;
		break;
	case MESSAGE_TYPE_ITEM_NAME:
		ss << M_ITEM_NAME;
		break;
	case MESSAGE_TYPE_ITEM_ICON_PATH:
		ss << M_ITEM_ICON_PATH;
		break;
	case MESSAGE_TYPE_ITEM_DESCRIPTION:
		ss << M_ITEM_DESCRIPTION;
		break;
	case MESSAGE_TYPE_ERROR_MESSAGE:
		ss << M_ERROR_MESSAGE;
		break;
	case MESSAGE_TYPE_SERVER_ANNOUNCEMENT:
		ss << M_SERVER_ANNOUNCEMENT;
		break;
	case MESSAGE_TYPE_USER_NAME:
		ss << M_USER_NAME;
		break;
	case MESSAGE_TYPE_KICKED:
		ss << M_KICKED;
		break;
	case MESSAGE_TYPE_LOAD_MAP:
		ss << M_LOAD_MAP;
		break;
	default:
		return "";
		break;
	}
	std::string finalString = word;
	std::replace(finalString.begin(), finalString.end(), ' ', '_');

	ss << " " << finalString << "*";
	return ss.str();
}

std::string NetworkMessageConverter::Convert(const unsigned int ID, const float fp)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_UPDATE_ACTOR:
		ss << M_UPDATE_ACTOR;
		break;
	case MESSAGE_TYPE_PING:
		ss << M_PING;
		break;
	case MESSAGE_TYPE_NEW_ACTOR:
		ss << M_NEW_ACTOR;
		break;
	case MESSAGE_TYPE_CLIENT_LATENCY:
		ss << M_CLIENT_LATENCY;
		break;
	case MESSAGE_TYPE_REMOVE_ACTOR:
		ss << M_REMOVE_ACTOR;
		break;
	case MESSAGE_TYPE_DEAD_ACTOR:
		ss << M_DEAD_ACTOR;
		break;
	case MESSAGE_TYPE_KEY_DOWN:
		ss << M_KEY_DOWN;
		break;
	case MESSAGE_TYPE_KEY_UP:
		ss << M_KEY_UP;
		break;
	case MESSAGE_TYPE_SERVER_UPDATES_PER_SEC:
		ss << M_SERVER_UPDATES_PER_SEC;
		break;
	case MESSAGE_TYPE_FRAME_TIME:
		ss << M_FRAME_TIME;
		break;
	case MESSAGE_TYPE_STATE:
		ss << M_STATE;
		break;
	case MESSAGE_TYPE_PLAY_SOUND:
		ss << M_PLAY_SOUND;
		break;
	case MESSAGE_TYPE_FOG_ENCLOSEMENT:
		ss << M_FOG_ENCLOSEMENT;
		break;
	case MESSAGE_TYPE_MESH_BINDING:
		ss << M_MESH_BINDING;
		break;
	case MESSAGE_TYPE_MESH_UNBIND:
		ss << M_MESH_UNBIND;
		break;
	case MESSAGE_TYPE_LOOT_OBJECT_RESPONSE:
		ss << M_LOOT_OBJECT_RESPONSE;
		break;
	case MESSAGE_TYPE_PICKUP_ITEM:
		ss << M_PICKUP_ITEM;
		break;
	case MESSAGE_TYPE_LOOT_ITEM:
		ss << M_LOOT_ITEM;
		break;
	case MESSAGE_TYPE_LOOT_OBJECT:
		ss << M_LOOT_OBJECT;
		break;
	case MESSAGE_TYPE_DROP_ITEM:
		ss << M_DROP_ITEM;
		break;
	case MESSAGE_TYPE_ITEM_TYPE:
		ss << M_ITEM_TYPE;
		break;
	case MESSAGE_TYPE_ITEM_SUB_TYPE:
		ss << M_ITEM_SUB_TYPE;
		break;
	case MESSAGE_TYPE_OBJECT_ID:
		ss << M_OBJECT_ID;
		break;
	case MESSAGE_TYPE_ITEM_STACK_SIZE:
		ss << M_ITEM_STACK_SIZE;
		break;
	case  MESSAGE_TYPE_ITEM_SLOT_SIZE:
		ss << M_ITEM_SLOT_SIZE;
		break;
	case MESSAGE_TYPE_ITEM_WEIGHT:
		ss << M_ITEM_WEIGHT;
		break;
	case MESSAGE_TYPE_WEAPON_DAMAGE:
		ss << M_WEAPON_DAMAGE;
		break;
	case MESSAGE_TYPE_WEAPON_RANGE:
		ss << M_WEAPON_RANGE;
		break;
	case MESSAGE_TYPE_ITEM_USE:
		ss << M_ITEM_USE;
		break;
	case MESSAGE_TYPE_ITEM_CRAFT:
		ss << M_ITEM_CRAFT;
		break;
	case MESSAGE_TYPE_ITEM_FILL:
		ss << M_ITEM_FILL;
		break;
	case MESSAGE_TYPE_EQUIPMENT_SLOT:
		ss << M_EQUIPMENT_SLOT;
		break;
	case MESSAGE_TYPE_EQUIP_ITEM:
		ss << M_EQUIP_ITEM;
		break;
	case MESSAGE_TYPE_REMOVE_EQUIPMENT:
		ss << M_REMOVE_EQUIPMENT;
		break;
	case MESSAGE_TYPE_UNEQUIP_ITEM:
		ss << M_UNEQUIP_ITEM;
		break;
	case MESSAGE_TYPE_WEAPON_USE:
		ss << M_WEAPON_USE;
		break;
	case MESSAGE_TYPE_CONTAINER_MAX:
		ss << M_CONTAINER_MAX;
		break;
	case MESSAGE_TYPE_CONTAINER_CURRENT:
		ss << M_CONTAINER_CURRENT;
		break;
	case MESSAGE_TYPE_PROJECTILE_DAMAGE:
		ss << M_PROJECTILE_DAMAGE;
		break;
	case MESSAGE_TYPE_PROJECTILE_VELOCITY:
		ss << M_PROJECTILE_VELOCITY;
		break;
	case MESSAGE_TYPE_REMOVE_INVENTORY_ITEM:
		ss << M_REMOVE_INVENTORY_ITEM;
		break;
	case MESSAGE_TYPE_HUNGER:
		ss << M_HUNGER;
		break;
	case MESSAGE_TYPE_HYDRATION:
		ss << M_HYDRATION;
		break;
	case MESSAGE_TYPE_HEALTH:
		ss << M_HEALTH;
		break;
	case MESSAGE_TYPE_ACTOR_TAKE_DAMAGE:
		ss << M_ACTOR_TAKE_DAMAGE;
		break;
	case MESSAGE_TYPE_PLAY_AS_ANIMAL:
		ss << M_PLAY_AS_ANIMAL;
		break;
	case MESSAGE_TYPE_TRY_TO_POSSESS_ANIMAL:
		ss << M_ATTEMPT_POSSESS_ANIMAL;
		break;
	case MESSAGE_TYPE_MAKE_NOISE:
		ss << M_MAKE_NOISE;
		break;
	case MESSAGE_TYPE_ANIMAL_ATTACK:
		ss << M_ANIMAL_ATTACK;
		break;
	case MESSAGE_TYPE_STAMINA:
		ss << M_STAMINA;
		break;
	case MESSAGE_TYPE_DEER_EAT_OBJECT:
		ss << M_DEER_EAT_OBJECT;
		break;
	case MESSAGE_TYPE_SELF_ID:
		ss << M_SELF_ID;
		break;
	case MESSAGE_TYPE_ACTOR_TYPE:
		ss << M_ACTOR_TYPE;
		break;
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << M_CONNECTION_CLOSED;
		break;
	case MESSAGE_TYPE_IMPORTANT_MESSAGE:
		ss << M_IMPORTANT_MESSAGE;
		break;
	case MESSAGE_TYPE_ACKNOWLEDGE:
		ss << M_ACKNOWLEDGE_MESSAGE;
		break;
	case MESSAGE_TYPE_PRINT_INVENTORY:
		ss << M_PRINT_INVENTORY;
		break;
	default:
		return "";
		break;
	}
	ss << " " << fp << "*";
	return ss.str();
}

std::string NetworkMessageConverter::Convert(const unsigned int ID)
{
	std::stringstream ss;
	switch (ID)
	{
	case MESSAGE_TYPE_CLIENT_DATA:
		ss << M_CLIENT_DATA;
		break;
	case MESSAGE_TYPE_ADD_INVENTORY_ITEM:
		ss << M_ADD_INVENTORY_ITEM;
		break;
	case MESSAGE_TYPE_ITEM_FINISHED:
		ss << M_ITEM_FINISHED;
		break;
	case MESSAGE_TYPE_ACTOR_KILL:
		ss << M_ACTOR_KILL;
		break;
	case MESSAGE_TYPE_LEAVE_ANIMAL:
		ss << M_LEAVE_ANIMAL;
		break;
	case MESSAGE_TYPE_RESTART_GAME_REQUEST:
		ss << M_RESTART_GAME_REQUEST;
		break;
	case MESSAGE_TYPE_SERVER_FULL:
		ss << M_SERVER_FULL;
		break;
	case MESSAGE_TYPE_USER_DATA:
		ss << M_USER_DATA;
		break;
	case  MESSAGE_TYPE_SERVER_SHUTDOWN:
		ss << M_SERVER_SHUTDOWN;
		break;
	case MESSAGE_TYPE_START_GAME:
		ss << M_START_GAME;
		break;
	case MESSAGE_TYPE_CONNECTED:
		ss << M_CONNECTED;
		break;
	case MESSAGE_TYPE_PLAYER_READY:
		ss << M_READY_PLAYER;
		break;
	case MESSAGE_TYPE_WEAPON_EQUIPMENT_SWAP:
		ss << M_WEAPON_EQUIPMENT_SWAP;
		break;
	case MESSAGE_TYPE_SERVER_RESTART:
		ss << M_SERVER_RESTART;
		break;
	default:
		return "";
		break;
	}
	ss << "*";
	return ss.str();
}

std::string NetworkMessageConverter::CombineMessage(const std::vector<std::string>& msgArray)
{
	std::string msg = "";
	for(unsigned int i = 0; i < msgArray.size(); i++)
	{
		msg += msgArray[i] + "*";
	}
	return msg;
}

std::vector<std::string> NetworkMessageConverter::SplitMessage(const std::string& msg)
{
	std::string subMsg = "";
	std::vector<std::string> msgArray;
	for(unsigned int i = 0; i < msg.length(); i++)
	{
		subMsg = "";
		while (msg.at(i) != '*' && i < msg.length())
		{
			subMsg += msg[i++];
		}
		msgArray.push_back(subMsg);
	}
	return msgArray;
}

Vector3 NetworkMessageConverter::ConvertStringToVector(const std::string& type, const std::string& msg)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	
	sscanf_s(msg.c_str(), (type + "%f %f %f").c_str(), &x, &y, &z);

	Vector3 vec = Vector3(x, y, z);
	
	return vec;
}

Vector4 NetworkMessageConverter::ConvertStringToQuaternion(const std::string& type, const std::string& msg)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
	sscanf_s(msg.c_str(), (type + "%f %f %f %f").c_str(), &x, &y, &z, &w);

	Vector4 quaternion = Vector4(x, y, z, w);

	return quaternion;
}

int NetworkMessageConverter::ConvertStringToInt(const std::string& type, const std::string& msg)
{
	int value = 0;
	sscanf_s(msg.c_str(), (type + "%d").c_str(), &value);
	return value;
}

float NetworkMessageConverter::ConvertStringToFloat(const std::string& type, const std::string& msg)
{
	float value = 0.0f;
	sscanf_s(msg.c_str(), (type + "%f").c_str(), &value);
	return value;
}

std::string NetworkMessageConverter::ConvertStringToSubstring( const std::string& type, const std::string& msg, bool bReplaceUnderscore /*= false*/ )
{
	char subString[100];
	sscanf_s(msg.c_str(), (type + "%s").c_str(), &subString, sizeof(subString));

	if (bReplaceUnderscore)
	{
		std::string ret = subString;

		std::replace(ret.begin(), ret.end(), '_', ' ');

		return ret;
	}
	return subString;
}