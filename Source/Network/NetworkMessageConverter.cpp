#include "NetworkMessageConverter.h"
#include <sstream>

NetworkMessageConverter::NetworkMessageConverter()
{
}

NetworkMessageConverter::~NetworkMessageConverter()
{
}

std::string NetworkMessageConverter::Convert( int ID, float x, float y, float z )
{
	stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_POSITION:
		ss << POSITION << x << "," << y << "," << z <<"*";
		break;
	case MESSAGE_TYPE_SCALE:
		ss << SCALE << x << "," << y << "," << z <<"*";
		break;
	case MESSAGE_TYPE_DIRECTION:
		ss << DIRECTION << x << "," << y << "," << z <<"*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}

std::string NetworkMessageConverter::Convert( int ID, float x, float y, float z, float w )
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_ROTATION:
		ss << ROTATION << x << "," << y << ","<<
			z << "," << w << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}

std::string NetworkMessageConverter::Convert( int ID, std::string fileName )
{
	std::stringstream ss;

	switch (ID)
	{
	case MESSAGE_TYPE_MESH_MODEL:
		ss << MESH_MODEL << fileName << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}

std::string NetworkMessageConverter::Convert( int ID, int state_ID )
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
	case MESSAGE_TYPE_CONNECTION_CLOSED:
		ss << CONNECTION_CLOSED << state_ID << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}

std::string NetworkMessageConverter::Convert( int ID )
{
	std::stringstream ss;
	switch (ID)
	{
	case MESSAGE_TYPE_SERVER_FULL:
		ss << SERVER_FULL << "*";
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
