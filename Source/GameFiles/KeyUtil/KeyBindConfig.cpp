#include "GameFiles/KeyUtil/KeyBindConfig.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>

/*Key words*/
static const std::string KEYWORD_BIND = "bind";
static const std::string KEYPRESS_FORWARD = "KP_FORWARD";
static const std::string KEYPRESS_BACKWARD = "KP_BACKWARD";
static const std::string KEYPRESS_LEFT = "KP_LEFT";
static const std::string KEYPRESS_RIGHT = "KP_RIGHT";
static const std::string KEYPRESS_INTERACT = "KP_USE";
static const std::string KEYPRESS_MENU = "KP_MENU";
static const std::string KEYPRESS_JUMP = "KP_JUMP";
static const std::string KEYPRESS_SPRINT = "KP_SPRINT";

/*Virtual Key Identifiers*/
//static const std::string VIRTUAL_ESCAPE


/*cfg path*/
static const std::string CONFIG_KEYBINDING_PATH = "KeyBindings.ini";


KeyBindConfig::KeyBindConfig()
{

}

KeyBindConfig::~KeyBindConfig()
{
}

void KeyBindConfig::ReadBindings( char* bindings )
{
	//if(!ReadFromFile(CONFIG_KEYBINDING_PATH, bindings))
	//{
		CreateDefaultIni(bindings);
		//return;
	//}
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
		VK_ESCAPE
	};

	memcpy(bindings, defaultBindings, sizeof(defaultBindings));
	//WriteToFile(CONFIG_KEYBINDING_PATH, defaultBindings);

}

bool KeyBindConfig::WriteToFile( const std::string path, const char* data )
{
	std::ofstream write;
	write.open(path);

	for (unsigned int i = 0; i < KEY_CAP; i++)
	{
		write<<data[i];

		if(i+1 < KEY_CAP)
			write<<"\n";
	}

	write.close();
	
	return true;
}

bool KeyBindConfig::ReadFromFile( const std::string path, char* data )
{
	
	int count = 0;
	int index = 0;

	std::ifstream read;
	read.open(path);

	if(!read.is_open())
		return false;

	while(!read.eof() && count < KEY_CAP)
	{
		char line[256]; 
		char key[20];

		read.getline(line,256);

		sscanf(line, "%s ", key);

		if(strcmp(key, KEYWORD_BIND.c_str()) == 0)
		{
			char keyBind[10];
			char keyCommand[20];

			sscanf(line, (KEYWORD_BIND + " %s %s").c_str(), keyBind, keyCommand);

			index = GetKeyValue(keyCommand);

		}

	}

	read.close();

	return true;
}

int KeyBindConfig::GetKeyValue( std::string KP )
{
	std::transform(KP.begin(), KP.end(), KP.begin(), ::toupper);

	if(KP == KEYPRESS_FORWARD)
	{
		return KEY_FORWARD;
	}

	if(KP == KEYPRESS_BACKWARD)
	{
		return KEY_BACKWARD;
	}

	if(KP == KEYPRESS_LEFT)
	{
		return KEY_LEFT;
	}

	if(KP == KEYPRESS_RIGHT)
	{
		return KEY_RIGHT;
	}

	if (KP == KEYPRESS_INTERACT)
	{
		return KEY_INTERACT;
	}

	if(KP == KEYPRESS_MENU)
	{
		return KEY_MENU;
	}

	if(KP == KEYPRESS_JUMP)
	{
		return KEY_JUMP;
	}

	if(KP == KEYPRESS_SPRINT)
	{
		return KEY_SPRINT;
	}

	return -1;
}







