/*
	Made by Edänge Simon And Christopher Jensen Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Vector.h"
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
	MESSAGE_TYPE_ERROR_MESSAGE,
	MESSAGE_TYPE_IMPORTANT_MESSAGE,
	MESSAGE_TYPE_ACKNOWLEDGE,
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
	MESSAGE_TYPE_CLIENT_DATA,
	MESSAGE_TYPE_PICKUP_ITEM,
	MESSAGE_TYPE_LOOT_ITEM,
	MESSAGE_TYPE_DROP_ITEM,
	MESSAGE_TYPE_HEALTH,
	MESSAGE_TYPE_HUNGER,
	MESSAGE_TYPE_HYDRATION,
	MESSAGE_TYPE_STAMINA,
	MESSAGE_TYPE_ITEM_TYPE,
	MESSAGE_TYPE_ITEM_ID,
	MESSAGE_TYPE_FETCH_INVENTORY,
	MESSAGE_TYPE_REMOVE_INVENTORY_ITEM,
	MESSAGE_TYPE_ADD_INVENTORY_ITEM,
	MESSAGE_TYPE_EQUIPMENT_SLOT,
	MESSAGE_TYPE_REMOVE_EQUIPMENT,
	MESSAGE_TYPE_ITEM_USE,
	MESSAGE_TYPE_ITEM_NAME,
	MESSAGE_TYPE_ITEM_DESCRIPTION,
	MESSAGE_TYPE_ITEM_ICON_PATH,
	MESSAGE_TYPE_ITEM_WEIGHT,
	MESSAGE_TYPE_ITEM_STACK_SIZE,
	MESSAGE_TYPE_WEAPON_DAMAGE,
	MESSAGE_TYPE_WEAPON_RANGE,
	MESSAGE_TYPE_CONTAINER_MAX,
	MESSAGE_TYPE_CONTAINER_CURRENT,
	MESSAGE_TYPE_WEAPON_USE,
	MESSAGE_TYPE_EQUIP_ITEM,
	MESSAGE_TYPE_UNEQUIP_ITEM,
	MESSAGE_TYPE_PROJECTILE_DAMAGE,
	MESSAGE_TYPE_PROJECTILE_VELOCITY,
	MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED,
	MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE,
	MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM,
	MESSAGE_TYPE_DEAD_PLAYER_REMOVE_ITEM,
	MESSAGE_TYPE_ADD_DEAD_PLAYER_OBJECT,
	MESSAGE_TYPE_REMOVE_DEAD_PLAYER_OBJECT,
	MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED,
	MESSAGE_TYPE_START_GAME
};
//Data Messages
static const std::string M_POSITION					= "P";
static const std::string M_ROTATION					= "R";
static const std::string M_DIRECTION				= "D";
static const std::string M_UP						= "U";
static const std::string M_MESH_MODEL				= "M";
static const std::string M_KICKED					= "K";
static const std::string M_SCALE					= "SC";
static const std::string M_HUNGER					= "HU";
static const std::string M_HEALTH					= "HE";
static const std::string M_HYDRATION				= "HY";
static const std::string M_FRAME_TIME				= "FT";
static const std::string M_STAMINA					= "STAM";
static const std::string M_STATE					= "STAT";
static const std::string M_ITEM_TYPE				= "IT";
static const std::string M_ITEM_ID					= "IID";
static const std::string M_ITEM_NAME				= "IN";
static const std::string M_ITEM_DESCRIPTION			= "IDE";
static const std::string M_ITEM_ICON_PATH			= "IIP";
static const std::string M_ITEM_WEIGHT				= "IW";
static const std::string M_ITEM_USE					= "IU";
static const std::string M_ITEM_STACK_SIZE			= "ISS";
static const std::string M_WEAPON_DAMAGE			= "WD";
static const std::string M_WEAPON_RANGE				= "WR";
static const std::string M_MATERIAL_CRAFTING_TYPE	= "MCT";
static const std::string M_MATERIAL_STACKS_REQUIRED	= "MSR";
static const std::string M_PROJECTILE_DAMAGE		= "PD";
static const std::string M_PROJECTILE_VELOCITY		= "PV";
static const std::string M_WEAPON_USE				= "WU";
static const std::string M_CONTAINER_CURRENT		= "COCU";
static const std::string M_CONTAINER_MAX			= "COMA";
//Item Messages
static const std::string M_EQUIPMENT_SLOT			= "ES";
static const std::string M_REMOVE_EQUIPMENT			= "RE";
static const std::string M_EQUIP_ITEM				= "EI";
static const std::string M_UNEQUIP_ITEM				= "UEI";
static const std::string M_PICKUP_ITEM				= "PI";
static const std::string M_LOOT_ITEM				= "LI";
static const std::string M_DROP_ITEM				= "DI";
static const std::string M_FETCH_INVENTORY			= "FI";
static const std::string M_ADD_INVENTORY_ITEM		= "AII";
static const std::string M_REMOVE_INVENTORY_ITEM	= "RII";
//Animal Messages
static const std::string M_NEW_ANIMAL				= "NA";
static const std::string M_REMOVE_ANIMAL			= "RA";
static const std::string M_UPDATE_ANIMAL			= "UA";
static const std::string M_DEAD_ANIMAL				= "DA";
//Key Messages
static const std::string M_KEY_DOWN					= "KD";
static const std::string M_KEY_UP					= "KU";
//Initial Data Message
static const std::string M_USER_DATA				= "UD";
//Shutdown Messages
static const std::string M_CONNECTION_CLOSED		= "CC";
static const std::string M_SERVER_FULL				= "SF";
static const std::string M_SERVER_SHUTDOWN			= "SS";
//Error Message
static const std::string M_ERROR_MESSAGE			= "EM";
//Important Message Tag
static const std::string M_IMPORTANT_MESSAGE		= "IM";
static const std::string M_ACKNOWLEDGE_MESSAGE		= "ACK";
//Player Messages
static const std::string M_CLIENT_DATA				= "CD";
static const std::string M_NEW_PLAYER				= "NP";
static const std::string M_UPDATE_PLAYER			= "UP";
static const std::string M_REMOVE_PLAYER			= "RP";
static const std::string M_DEAD_PLAYER				= "DP";
static const std::string M_ADD_DEAD_PLAYER_ITEM		= "ADI";
static const std::string M_DEAD_PLAYER_ADD_ITEM		= "DPAI";
static const std::string M_DEAD_PLAYER_REMOVE_ITEM	= "DPRI";
static const std::string M_ADD_DEAD_PLAYER_OBJECT	= "ADPO";
static const std::string M_REMOVE_DEAD_PLAYER_OBJECT= "RDPO";
static const std::string M_DEAD_PLAYER_ITEM_FINISHED= "DPIF";
//Static Object Messages
static const std::string M_NEW_STATIC_OBJECT		= "NSO";
static const std::string M_REMOVE_STATIC_OBJECT		= "RSO";
static const std::string M_UPDATE_STATIC_OBJECT		= "USO";
//Dynamic Object Messages
static const std::string M_NEW_DYNAMIC_OBJECT		= "NDO";
static const std::string M_REMOVE_DYNAMIC_OBJECT	= "RDO";
static const std::string M_UPDATE_DYNAMIC_OBJECT	= "UDO";
//Confirmation Message
static const std::string M_SELF_ID					= "SELF";
//Ping Message
static const std::string M_PING						= "PING";

static const std::string M_START_GAME				= "SG";

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

	/*! Converts parameters to correctly formated strings sent between client and server.*/
	std::string Convert(const unsigned int ID);
	std::string Convert(const unsigned int ID, const float fp);
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