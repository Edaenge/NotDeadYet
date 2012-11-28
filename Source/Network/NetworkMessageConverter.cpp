#include "NetworkMessageConverter.h"
#include <sstream>

NetworkMessageConverter::NetworkMessageConverter()
{
}
NetworkMessageConverter::~NetworkMessageConverter()
{
}
std::string NetworkMessageConverter::Convert(const int ID, const float x, const float y, const float z)
{
	stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_POSITION:
		ss << POSITION << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_SCALE:
		ss << SCALE << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_DIRECTION:
		ss << DIRECTION << x << " " << y << " " << z <<"*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const int ID, const float x, const float y, const float z, const float w)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_ROTATION:
		ss << ROTATION << x << " " << y << " "<<
			z << " " << w << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const int ID, const std::string word)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_MESH_MODEL:
		ss << MESH_MODEL << word << "*";
		break;
	case MESSAGE_TYPE_KEY_PRESS:
		ss << KEY_PRESS << word << "*";
		break;
	case  MESSAGE_TYPE_KICKED:
		ss << KICKED << word <<"*";
		break;
	case MESSAGE_TYPE_USER_DATA:
		ss << USER_DATA << word << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const int ID, const int state_ID)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_PLAYER_UPDATE:
		ss << PLAYER_UPDATE << state_ID << "*";
		break;
	case MESSAGE_TYPE_STATE:
		ss << STATE << state_ID << "*";
		break;
	case MESSAGE_TYPE_NEW_PLAYER:
		ss << NEW_PLAYER << state_ID << "*";
		break;
	case MESSAGE_TYPE_REMOVE_PLAYER:
		ss << REMOVE_PLAYER << state_ID << "*";
		break;
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << CONNECTION_CLOSED << state_ID << "*";
		break;
	case MESSAGE_TYPE_SELF_ID:
		ss << SELF_ID << state_ID << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const int ID)
{
	std::stringstream ss;
	switch (ID)
	{
	case MESSAGE_TYPE_SERVER_FULL:
		ss << SERVER_FULL << "*";
		break;
	case MESSAGE_TYPE_PING:
		ss << PING << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::CombineMessage(std::vector<std::string> msgArray)
{
	std::string msg = "";
	for(int i = 0; i < msgArray.size(); i++)
	{
		msg += msgArray[i] + "*";
	}
	return msg;
}
vector<std::string> NetworkMessageConverter::SplitMessage(std::string msg)
{
	std::string subMsg = "";
	std::vector<std::string> msgArray;
	for(int i = 0; i < msg.length(); i++)
	{
		subMsg = "";
		while (msg.at(i) != '*')
		{
			subMsg += msg.at(i++);
		}
		msgArray.push_back(subMsg);
	}
	return msgArray;
}
D3DXVECTOR3 NetworkMessageConverter::ConvertStringToVector(const std::string type, std::string msg)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	sscanf_s(msg.c_str(), (type + "%f %f %f").c_str(), &x, &y, &z);

	D3DXVECTOR3 vec = D3DXVECTOR3(x, y, z);
	
	return vec;
}
D3DXQUATERNION NetworkMessageConverter::ConvertStringToQuaternion(const std::string type, std::string msg)
{
	float x;
	float y;
	float z;
	float w;
	sscanf_s(msg.c_str(), (type + "%f %f %f %f").c_str(), &x, &y, &z, &w);

	D3DXQUATERNION quaternion = D3DXQUATERNION(x, y, z, w);

	return quaternion;
}
int NetworkMessageConverter::ConvertStringToInt(const std::string type, std::string msg)
{
	int value;
	sscanf_s(msg.c_str(), (type + "%d").c_str(), &value);

	return value;
}
std::string NetworkMessageConverter::ConvertStringToSubstring(const std::string type, std::string msg)
{
	char subString[100];

	sscanf(msg.c_str(), (type + "%s").c_str(), &subString);

	return subString;
}
