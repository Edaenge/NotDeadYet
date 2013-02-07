#include "NetworkMessageConverter.h"
#include <sstream>


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
		ss << M_UP << " " << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_DIRECTION:
		ss << M_DIRECTION << " " << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_POSITION:
		ss << M_POSITION << " " << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_SCALE:
		ss << M_SCALE << " " << x << " " << y << " " << z <<"*";
		break;
	default:
		ss << "";
		break;
	}

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
	case MESSAGE_TYPE_LOAD_MAP:
		ss << M_LOAD_MAP;
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
	case MESSAGE_TYPE_KICKED:
		ss << M_KICKED;
		break;
	case MESSAGE_TYPE_ERROR_MESSAGE:
		ss << M_ERROR_MESSAGE;
		break;
	default:
		return "";
		break;
	}
	ss << " " << word << "*";
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
	case MESSAGE_TYPE_NEW_ACTOR:
		ss << M_NEW_ACTOR;
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
	case MESSAGE_TYPE_FRAME_TIME:
		ss << M_FRAME_TIME;
		break;
	case MESSAGE_TYPE_STATE:
		ss << M_STATE;
		break;
	case MESSAGE_TYPE_PICKUP_ITEM:
		ss << M_PICKUP_ITEM;
		break;
	case MESSAGE_TYPE_LOOT_ITEM:
		ss << M_LOOT_ITEM;
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
	case MESSAGE_TYPE_ITEM_ID:
		ss << M_ITEM_ID;
		break;
	case MESSAGE_TYPE_ADD_INVENTORY_ITEM:
		ss << M_ADD_INVENTORY_ITEM;
		break;
	case MESSAGE_TYPE_ITEM_STACK_SIZE:
		ss << M_ITEM_STACK_SIZE;
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
	case MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED:
		ss << M_MATERIAL_STACKS_REQUIRED;
		break;
	case MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE:
		ss << M_MATERIAL_CRAFTING_TYPE;
		break;
	case MESSAGE_TYPE_ITEM_USE:
		ss << M_ITEM_USE;
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
	case MESSAGE_TYPE_STAMINA:
		ss << M_STAMINA;
		break;
	case MESSAGE_TYPE_DEAD_ANIMAL_ADD_ITEM:
		ss << M_ADD_DEAD_ANIMAL_ITEM;
		break;
	case MESSAGE_TYPE_DEAD_ANIMAL_REMOVE_ITEM:
		ss << M_DEAD_ANIMAL_REMOVE_ITEM;
		break;
	case MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM:
		ss << M_DEAD_PLAYER_ADD_ITEM;
		break;
	case MESSAGE_TYPE_DEAD_PLAYER_REMOVE_ITEM:
		ss << M_DEAD_PLAYER_REMOVE_ITEM;
		break;
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << M_CONNECTION_CLOSED;
		break;
	case MESSAGE_TYPE_SELF_ID:
		ss << M_SELF_ID;
		break;
	case MESSAGE_TYPE_IMPORTANT_MESSAGE:
		ss << M_IMPORTANT_MESSAGE;
		break;
	case MESSAGE_TYPE_ACKNOWLEDGE:
		ss << M_ACKNOWLEDGE_MESSAGE;
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
	case MESSAGE_TYPE_PING:
		ss << M_PING;
		break;
	case MESSAGE_TYPE_DEAD_ANIMAL_ITEM_FINISHED:
		ss << M_DEAD_ANIMAL_ITEM_FINISHED;
		break;
	case MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED:
		ss << M_DEAD_PLAYER_ITEM_FINISHED;
		break;;
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
		while (msg.at(i) != '*' || i >= msg.length())
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
	float x;
	float y;
	float z;
	float w;
	sscanf_s(msg.c_str(), (type + "%f %f %f %f").c_str(), &x, &y, &z, &w);

	Vector4 quaternion = Vector4(x, y, z, w);

	return quaternion;
}
int NetworkMessageConverter::ConvertStringToInt(const std::string& type, const std::string& msg)
{
	int value;
	sscanf_s(msg.c_str(), (type + "%d").c_str(), &value);
	return value;
}
float NetworkMessageConverter::ConvertStringToFloat(const std::string& type, const std::string& msg)
{
	float value;
	sscanf_s(msg.c_str(), (type + "%f").c_str(), &value);
	return value;
}
std::string NetworkMessageConverter::ConvertStringToSubstring(const std::string& type, const std::string& msg)
{
	char subString[100];
	sscanf_s(msg.c_str(), (type + "%s").c_str(), &subString, sizeof(subString));
	return subString;

}