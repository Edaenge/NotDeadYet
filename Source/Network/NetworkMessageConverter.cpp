#include "NetworkMessageConverter.h"
#include <sstream>


NetworkMessageConverter::NetworkMessageConverter()
{
}
NetworkMessageConverter::~NetworkMessageConverter()
{
}
std::string NetworkMessageConverter::Convert(const unsigned int ID, const float x, const float y, const float z)
{
	std::stringstream ss;

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
std::string NetworkMessageConverter::Convert(const unsigned int ID, const float x, const float y, const float z, const float w)
{
	std::stringstream ss;

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
std::string NetworkMessageConverter::Convert(const unsigned int ID, const std::string word)
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
	case MESSAGE_TYPE_UPDATE_PLAYER:
		ss << M_UPDATE_PLAYER;
		break;
	case MESSAGE_TYPE_UPDATE_STATIC_OBJECT:
		ss << M_UPDATE_STATIC_OBJECT;
		break;
	case MESSAGE_TYPE_UPDATE_DYNAMIC_OBJECT:
		ss << M_UPDATE_DYNAMIC_OBJECT;
		break;
	case MESSAGE_TYPE_UPDATE_ANIMAL:
		ss << M_UPDATE_ANIMAL;
		break;
	case MESSAGE_TYPE_DEAD_PLAYER:
		ss << M_DEAD_PLAYER;
		break;
	case MESSAGE_TYPE_DEAD_ANIMAL:
		ss << M_DEAD_ANIMAL;
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
	case MESSAGE_TYPE_NEW_PLAYER:
		ss << M_NEW_PLAYER;
		break;
	case MESSAGE_TYPE_NEW_STATIC_OBJECT:
		ss << M_NEW_STATIC_OBJECT;
		break;
	case MESSAGE_TYPE_NEW_DYNAMIC_OBJECT:
		ss << M_NEW_DYNAMIC_OBJECT;
		break;
	case MESSAGE_TYPE_NEW_ANIMAL:
		ss << M_NEW_ANIMAL;
		break;
	case MESSAGE_TYPE_PICKUP_ITEM:
		ss << M_PICKUP_ITEM;
		break;
	case MESSAGE_TYPE_DROP_ITEM:
		ss << M_DROP_ITEM;
		break;
	case MESSAGE_TYPE_ITEM_TYPE:
		ss << M_ITEM_TYPE;
		break;
	case MESSAGE_TYPE_ADD_INVENTORY_ITEM:
		ss << M_ADD_INVENTORY_ITEM;
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
	case MESSAGE_TYPE_CONTAINER_MAX:
		ss << M_CONTAINER_MAX;
		break;
	case MESSAGE_TYPE_CONTAINER_CURRENT:
		ss << M_CONTAINER_CURRENT;
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
	case MESSAGE_TYPE_REMOVE_PLAYER:
		ss << M_REMOVE_PLAYER;
		break;
	case MESSAGE_TYPE_REMOVE_STATIC_OBJECT:
		ss << M_REMOVE_STATIC_OBJECT;
		break;
	case MESSAGE_TYPE_REMOVE_ANIMAL:
		ss << M_REMOVE_ANIMAL;
		break;
	case MESSAGE_TYPE_REMOVE_DYNAMIC_OBJECT:
		ss << M_REMOVE_DYNAMIC_OBJECT;
		break;
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << M_CONNECTION_CLOSED;
		break;
	case MESSAGE_TYPE_SELF_ID:
		ss << M_SELF_ID;
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
	case MESSAGE_TYPE_FETCH_INVENTORY:
		ss << M_FETCH_INVENTORY;
		break;
	case MESSAGE_TYPE_SERVER_FULL:
		ss << M_SERVER_FULL;
		break;
	case MESSAGE_TYPE_USER_DATA:
		ss << M_USER_DATA;
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