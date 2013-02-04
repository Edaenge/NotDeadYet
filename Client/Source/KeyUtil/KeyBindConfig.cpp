#include "KeyBindConfig.h"
#include "KeyConverter.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>


/*Key words*/
static const std::string COMMAND_FORWARD	= "C_FORWARD";
static const std::string COMMAND_BACKWARD	= "C_BACKWARD";
static const std::string COMMAND_LEFT		= "C_LEFT";
static const std::string COMMAND_RIGHT		= "C_RIGHT";
static const std::string COMMAND_INTERACT	= "C_USE";
static const std::string COMMAND_MENU		= "C_MENU";
static const std::string COMMAND_JUMP		= "C_JUMP";
static const std::string COMMAND_SPRINT		= "C_SPRINT";
static const std::string COMMAND_DUCK		= "C_DUCK";
static const std::string COMMAND_INVENTORY	= "C_INVENTORY";
static const std::string COMMAND_EQUIPMENT	= "C_EQUIPMENT";


/*cfg path*/
static const std::string CONFIG_KEYBINDING_PATH = "KeyBindings.ini";


KeyBindConfig::KeyBindConfig()
{

}

KeyBindConfig::~KeyBindConfig()
{
}

void KeyBindConfig::ReadBindings(char* bindings)
{
	if(!ReadFromFile(CONFIG_KEYBINDING_PATH, bindings))
	{
		CreateDefaultIni(bindings);
		return;
	}
}

void KeyBindConfig::CreateDefaultIni(char* bindings)
{
	char defaultBindings[] =
	{
		'W',
		'S',
		'A',
		'D',
		VK_SHIFT,
		VK_SPACE,
		VK_CONTROL,
		'E',
		VK_ESCAPE,
		'B',
		'C'
	};

	memcpy(bindings, defaultBindings, sizeof(defaultBindings));
	WriteToFile(CONFIG_KEYBINDING_PATH, defaultBindings);

}

bool KeyBindConfig::WriteToFile( const std::string path, const char* data )
{
	std::ofstream write;
	std::string txt;
	write.open(path);

	for (unsigned int i = 0; i < KEY_CAP; i++)
	{
		
		//Find the key string
	
		if(ConvertKey(txt, data[i]))
			write<<txt;

		else
			write<<data[i];
		
		write<<"="<<GetCommand(i);

		if(i+1 < KEY_CAP)
			write<<"\n";
	}

	write.close();
	
	return true;
}

bool KeyBindConfig::ConvertKey(std::string& ret, const char key) const
{
	KeyConverter kc;


	//Identify the key
	if(kc.CheckCommonKeys(ret, key))
		return true;
	if(kc.CheckNumPad(ret,key))
		return true;
	if(kc.CheckFKeys(ret,key))
		return true;
	if (kc.CheckUncommonKeys(ret,key))
		return true;
		
	
	return false;
	
}

bool KeyBindConfig::ReadFromFile(const std::string path, char* data)
{
	KeyConverter kc;
	int count = 0;
	int index = 0;

	std::ifstream read;
	read.open(path);

	if(!read.is_open())
		return false;

	while(!read.eof() && count < KEY_CAP)
	{
		char line[256]; 
		char key[52];
		char command[52];

		read.getline(line,256);
		TrimAndSet(line);
		sscanf_s(line, "%s = ", &key, sizeof(key));
		sscanf_s(line, (std::string(key) + " = %s").c_str(), &command, sizeof(command));

		index = GetKeyValue(command);

		char keyAdd = kc.ConvertToChar(key);

		if(keyAdd == '\0')
			keyAdd = key[0];
		
		if(index != -1)
			data[index] = keyAdd;

		count++;
	}

	read.close();

	return true;
}

int KeyBindConfig::GetKeyValue(std::string CC)
{
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);

	if(CC == COMMAND_FORWARD)
	{
		return KEY_FORWARD;
	}

	if(CC == COMMAND_BACKWARD)
	{
		return KEY_BACKWARD;
	}

	if(CC == COMMAND_LEFT)
	{
		return KEY_LEFT;
	}

	if(CC == COMMAND_RIGHT)
	{
		return KEY_RIGHT;
	}

	if (CC == COMMAND_INTERACT)
	{
		return KEY_INTERACT;
	}

	if(CC == COMMAND_MENU)
	{
		return KEY_MENU;
	}

	if(CC == COMMAND_JUMP)
	{
		return KEY_JUMP;
	}

	if(CC == COMMAND_SPRINT)
	{
		return KEY_SPRINT;
	}

	if(CC == COMMAND_DUCK)
	{
		return KEY_DUCK;
	}

	if(CC == COMMAND_INVENTORY)
	{
		return KEY_INVENTORY;
	}

	return -1;
}

const std::string& KeyBindConfig::GetCommand(const int keyValue)
{
	switch (keyValue)
	{
	case KEY_FORWARD:
		return COMMAND_FORWARD;
		break;
	case KEY_BACKWARD:
		return COMMAND_BACKWARD;
		break;
	case KEY_LEFT:
		return COMMAND_LEFT;
		break;
	case KEY_RIGHT:
		return COMMAND_RIGHT;
		break;
	case KEY_SPRINT:
		return COMMAND_SPRINT;
		break;
	case KEY_JUMP:
		return COMMAND_JUMP;
		break;
	case KEY_DUCK:
		return COMMAND_DUCK;
		break;
	case KEY_INTERACT:
		return COMMAND_INTERACT;
		break;
	case KEY_MENU:
		return COMMAND_MENU;
		break;
	case KEY_INVENTORY:
		return COMMAND_INVENTORY;
		break;
	case KEY_EQUIPMENT:
		return COMMAND_EQUIPMENT;
		break;
	default:
		return "none";
		break;
	}
}

void KeyBindConfig::TrimAndSet(char* ret)
{
	if(ret == NULL)
		return;

	std::string str(ret);
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

	size_t found;
	found = str.find("=");

	if(found == std::string::npos)
		return;

	int index = (int)found;

	std::string subA, subB;
	subA = str.substr(0,index);
	subB = str.substr(index+1,str.size()-1);

	subA += " = ";
	str = subA + subB;

	strcpy(ret, str.c_str());
	
}










