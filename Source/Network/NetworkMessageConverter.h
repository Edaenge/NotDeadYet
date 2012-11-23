/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
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
	MESSAGE_TYPE_PLAYER_UPDATE,
	MESSAGE_TYPE_NEW_PLAYER
};

static const std::string POSITION			= "P:";
static const std::string ROTATION			= "R:";
static const std::string SCALE				= "Sc:";
static const std::string STATE				= "St:";
static const std::string DIRECTION			= "D:";
static const std::string MESH_MODEL			= "M:";
static const std::string CONNECTION_CLOSED	= "CC:";
static const std::string SERVER_FULL		= "SF:";
static const std::string NEW_PLAYER			= "NP:";
static const std::string PLAYER_UPDATE		= "PU:";

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
	std::string Convert(int ID);
	std::string Convert(int ID, int state_ID);
	std::string Convert(int ID, std::string filename);
	std::string Convert(int ID, float x, float y, float z);
	std::string Convert(int ID, float x, float y, float z, float w);
	

private:


};