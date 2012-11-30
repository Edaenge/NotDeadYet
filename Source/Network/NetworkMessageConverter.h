/*
	Made by Edänge Simon And Christopher Jensen Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "stdafx.h"

enum MESSAGE_TYPE
{
	MESSAGE_TYPE_POSITION,
	MESSAGE_TYPE_DIRECTION,
	MESSAGE_TYPE_SCALE,
	MESSAGE_TYPE_ROTATION,
	MESSAGE_TYPE_STATE,
	MESSAGE_TYPE_MESH_MODEL,
	MESSAGE_TYPE_CONNECTION_CLOSED,
	MESSAGE_TYPE_SERVER_FULL,
	MESSAGE_TYPE_SERVER_SHUTDOWN,
	MESSAGE_TYPE_PLAYER_UPDATE,
	MESSAGE_TYPE_NEW_PLAYER,
	MESSAGE_TYPE_KEY_PRESS,
	MESSAGE_TYPE_PING,
	MESSAGE_TYPE_SELF_ID,
	MESSAGE_TYPE_KICKED,
	MESSAGE_TYPE_REMOVE_PLAYER,
	MESSAGE_TYPE_USER_DATA
};

static const std::string POSITION			= "P ";
static const std::string ROTATION			= "R ";
static const std::string DIRECTION			= "D ";
static const std::string MESH_MODEL			= "M ";
static const std::string KICKED				= "K ";
static const std::string SCALE				= "Sc ";
static const std::string STATE				= "St ";
static const std::string CONNECTION_CLOSED	= "CC ";
static const std::string SERVER_FULL		= "SF ";
static const std::string SERVER_SHUTDOWN	= "SS ";
static const std::string NEW_PLAYER			= "NP ";
static const std::string PLAYER_UPDATE		= "PU ";
static const std::string KEY_PRESS			= "KP ";
static const std::string REMOVE_PLAYER		= "RP ";
static const std::string USER_DATA			= "UD ";
static const std::string PING				= "Ping ";
static const std::string SELF_ID			= "Self ";
/*
	This class creates string messages used for communication
	between sockets.
	Returns an empty string if wrong function is used.
*/
class NetworkMessageConverter
{
public:
	NetworkMessageConverter();
	virtual ~NetworkMessageConverter();

	//Converts parameters to correctly formated strings sent between client and server
	std::string Convert(const int ID);
	std::string Convert(const int ID, const int state_ID);
	std::string Convert(const int ID, const std::string word);
	std::string Convert(const int ID, const float x, const float y, const float z);
	std::string Convert(const int ID, const float x, const float y, const float z, const float w);
	
	std::string CombineMessage(std::vector<std::string> msgArray);
	std::vector<std::string> SplitMessage(std::string msg);
	D3DXVECTOR3 ConvertStringToVector(const std::string type, std::string msg);
	D3DXQUATERNION ConvertStringToQuaternion(const std::string type, std::string msg);
	int ConvertStringToInt(const std::string type, std::string msg);
	std::string ConvertStringToSubstring(const std::string type, std::string msg);
private:


};