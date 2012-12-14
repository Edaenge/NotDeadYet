/*
	Made by Edänge Simon And Christopher Jensen Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <Vector.h>
#include <vector>
#include <string>

enum MESSAGE_TYPE
{
	MESSAGE_TYPE_POSITION,
	MESSAGE_TYPE_DIRECTION,
	MESSAGE_TYPE_UP,
	MESSAGE_TYPE_SCALE,
	MESSAGE_TYPE_ROTATION,
	MESSAGE_TYPE_STATE,
	MESSAGE_TYPE_MESH_MODEL,
	MESSAGE_TYPE_CONNECTION_CLOSED,
	MESSAGE_TYPE_SERVER_FULL,
	MESSAGE_TYPE_SERVER_SHUTDOWN,
	MESSAGE_TYPE_UPDATE_PLAYER,
	MESSAGE_TYPE_DEAD_PLAYER,
	MESSAGE_TYPE_UPDATE_STATIC_OBJECT,
	MESSAGE_TYPE_UPDATE_DYNAMIC_OBJECT,
	MESSAGE_TYPE_UPDATE_ANIMAL,
	MESSAGE_TYPE_DEAD_ANIMAL,
	MESSAGE_TYPE_NEW_PLAYER,
	MESSAGE_TYPE_NEW_STATIC_OBJECT,
	MESSAGE_TYPE_NEW_DYNAMIC_OBJECT,
	MESSAGE_TYPE_NEW_ANIMAL,
	MESSAGE_TYPE_KEY_DOWN,
	MESSAGE_TYPE_KEY_UP,
	MESSAGE_TYPE_PING,
	MESSAGE_TYPE_SELF_ID,
	MESSAGE_TYPE_KICKED,
	MESSAGE_TYPE_REMOVE_PLAYER,
	MESSAGE_TYPE_REMOVE_STATIC_OBJECT,
	MESSAGE_TYPE_REMOVE_DYNAMIC_OBJECT,
	MESSAGE_TYPE_REMOVE_ANIMAL,
	MESSAGE_TYPE_USER_DATA,
	MESSAGE_TYPE_FRAME_TIME,
	MESSAGE_TYPE_CLIENT_DATA
};
//Data Messages
static const std::string POSITION				= "P";
static const std::string ROTATION				= "R";
static const std::string DIRECTION				= "D";
static const std::string UP						= "U";
static const std::string MESH_MODEL				= "M";
static const std::string KICKED					= "K";
static const std::string SCALE					= "Sc";
static const std::string STATE					= "St";
//Shutdown Messages
static const std::string CONNECTION_CLOSED		= "CC";
static const std::string SERVER_FULL			= "SF";
static const std::string SERVER_SHUTDOWN		= "SS";
//Player Messages
static const std::string CLIENT_DATA			= "CD";
static const std::string NEW_PLAYER				= "NP";
static const std::string UPDATE_PLAYER			= "UP";
static const std::string REMOVE_PLAYER			= "RP";
static const std::string DEAD_PLAYER			= "DP";
//Static Object Messages
static const std::string NEW_STATIC_OBJECT		= "NSO";
static const std::string REMOVE_STATIC_OBJECT	= "RSO";
static const std::string UPDATE_STATIC_OBJECT	= "USO";
//Dynamic Object Messages
static const std::string NEW_DYNAMIC_OBJECT		= "NDO";
static const std::string REMOVE_DYNAMIC_OBJECT	= "RDO";
static const std::string UPDATE_DYNAMIC_OBJECT	= "UDO";
//Animal Messages
static const std::string NEW_ANIMAL				= "NA";
static const std::string REMOVE_ANIMAL			= "RA";
static const std::string UPDATE_ANIMAL			= "UA";
static const std::string DEAD_ANIMAL			= "DA";
//Key Messages
static const std::string KEY_DOWN				= "KD";
static const std::string KEY_UP					= "KU";
//Initial Data Message
static const std::string USER_DATA				= "UD";
//Confirmation Message
static const std::string SELF_ID				= "Self";
//Ping Message
static const std::string PING					= "Ping";
static const std::string FRAME_TIME				= "FT";
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
	std::string Convert(const unsigned int ID);
	//std::string Convert(const unsigned int ID, const unsigned int state_ID);
	std::string Convert(const unsigned int ID, const float time);
	std::string Convert(const unsigned int ID, const std::string word);
	std::string Convert(const unsigned int ID, const float x, const float y, const float z);
	std::string Convert(const unsigned int ID, const float x, const float y, const float z, const float w);
	
	std::string CombineMessage(const std::vector<std::string>& msgArray);
	std::vector<std::string> SplitMessage(const std::string& msg);
	Vector3 ConvertStringToVector(const std::string& type, const std::string& msg);
	Vector4 ConvertStringToQuaternion(const std::string& type, const std::string& msg);
	int ConvertStringToInt(const std::string& type, const std::string& msg);
	float ConvertStringToFloat(const std::string& type, const std::string& msg);
	std::string ConvertStringToSubstring(const std::string& type, const std::string& msg);
private:


};