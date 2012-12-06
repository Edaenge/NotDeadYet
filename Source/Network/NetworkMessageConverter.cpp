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
	case MESSAGE_TYPE_POSITION:
		ss << POSITION << " " << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_SCALE:
		ss << SCALE << " " << x << " " << y << " " << z <<"*";
		break;
	case MESSAGE_TYPE_DIRECTION:
		ss << DIRECTION << " " << x << " " << y << " " << z <<"*";
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
		ss << ROTATION << " " << x << " " << y << " "<<
			z << " " << w << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const unsigned int ID, const std::string word)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_MESH_MODEL:
		ss << MESH_MODEL << " " << word << "*";
		break;
	case MESSAGE_TYPE_KEY_DOWN:
		ss << KEY_DOWN << " " << word << "*";
		break;
	case MESSAGE_TYPE_KEY_UP:
		ss << KEY_UP << " " << word << "*";
		break;
	case  MESSAGE_TYPE_KICKED:
		ss << KICKED << " " << word <<"*";
		break;
	case MESSAGE_TYPE_USER_DATA:
		ss << USER_DATA << " " << word << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const unsigned int ID, const unsigned int state_ID)
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_UPDATE_PLAYER:
		ss << UPDATE_PLAYER << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_KEY_DOWN:
		ss << KEY_DOWN << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_KEY_UP:
		ss << KEY_UP << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_STATE:
		ss << STATE << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_NEW_PLAYER:
		ss << NEW_PLAYER << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_REMOVE_PLAYER:
		ss << REMOVE_PLAYER << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << CONNECTION_CLOSED << " " << state_ID << "*";
		break;
	case MESSAGE_TYPE_SELF_ID:
		ss << SELF_ID << " " << state_ID << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
std::string NetworkMessageConverter::Convert(const unsigned int ID)
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
std::string NetworkMessageConverter::CombineMessage(const std::vector<std::string>& msgArray)
{
	std::string msg = "";
	for(unsigned int i = 0; i < msgArray.size(); i++)
	{
		msg += msgArray[i] + "*";
	}
	return msg;
}
std::vector<std::string> NetworkMessageConverter::SplitMessage(std::string msg)
{
	std::string subMsg = "";
	std::vector<std::string> msgArray;
	for(unsigned int i = 0; i < msg.length(); i++)
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
Vector3 NetworkMessageConverter::ConvertStringToVector(const std::string& type, std::string msg)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	sscanf_s(msg.c_str(), (type + "%f %f %f").c_str(), &x, &y, &z);

	Vector3 vec = Vector3(x, y, z);
	
	return vec;
}
Vector4 NetworkMessageConverter::ConvertStringToQuaternion(const std::string& type, std::string msg)
{
	float x;
	float y;
	float z;
	float w;
	sscanf_s(msg.c_str(), (type + "%f %f %f %f").c_str(), &x, &y, &z, &w);

	Vector4 quaternion = Vector4(x, y, z, w);

	return quaternion;
}
int NetworkMessageConverter::ConvertStringToInt(const std::string& type, std::string msg)
{
	int value;
	sscanf_s(msg.c_str(), (type + "%d").c_str(), &value);
	return value;
}
std::string NetworkMessageConverter::ConvertStringToSubstring(const std::string& type, std::string msg)
{
	char subString[100];
	sscanf(msg.c_str(), (type + "%s").c_str(), &subString);
	return subString;
}
