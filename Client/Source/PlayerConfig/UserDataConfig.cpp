#include "UserDataConfig.h"

static const std::string PATH			= "UserData.cfg";

static const std::string player_Settings = "[PlayerSettings]";
static const std::string playerName		= "NAME";
static const std::string playerModel	= "MODEL";
static const std::string DefaultName	= "Xavier";
static const std::string DefaultModel	= "Media/Models/token_anims.fbx";

static const std::string END			= "#END";

UserDataConfig::UserDataConfig()
{
	zUserName = "";
	zUserModel = "";
}

UserDataConfig::~UserDataConfig()
{

}

bool UserDataConfig::ReadFromFile()
{

	std::ifstream read;
	read.open(PATH.c_str());

	if(!read.is_open())
		return false;
	

	while(!read.eof())
	{
		std::string line;
		char key[52];
		char command[52];

		std::getline(read, line);

		if(line.empty())
			continue;

		sscanf_s(line.c_str(), "%s", command, sizeof(command));

		if(strcmp(command, player_Settings.c_str()) == 0)
		{
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s", command, sizeof(command), key, sizeof(key));

				if(command != END)
					InterpCommand(command, key);
			}
		}
		
	}

	read.close();

	return true;
}

bool UserDataConfig::SaveUserDataCfg()
{
	std::vector<std::string> data;

	data.push_back(player_Settings);
	if(zUserName.empty())
		data.push_back(playerName +"="+ DefaultName);
	else
		data.push_back(playerName +"="+ this->zUserName);

	if(zUserModel.empty())
		data.push_back(playerModel +"="+ DefaultModel);
	else
		data.push_back(playerModel +"="+ this->zUserModel);

	data.push_back(END);

	return true;
}

bool UserDataConfig::InterpCommand( std::string command, std::string key )
{
	if(command.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == playerName)
	{
		if(!key.empty())
			this->zUserName = key;
		else
			this->zUserName = DefaultName;
	}
	else if(command == playerModel)
	{
		if(!key.empty())
			this->zUserModel = key;
		else
			this->zUserModel = key;
	}

	return true;
}

void UserDataConfig::CreateDefaultCfg()
{
	std::vector<std::string> data;

	
	data.push_back(player_Settings);
	data.push_back(playerName +"="+ DefaultName);
	data.push_back(playerModel +"="+ DefaultModel);
	data.push_back(END);

	//Add more def. values if needed.
	

	WriteToFile(data);
}

void UserDataConfig::WriteToFile( std::vector<std::string>& data )
{
	std::ofstream write;
	write.open(PATH.c_str());

	for (auto it = data.begin(); it != data.end(); it++)
	{
		write << (*it)+"\n";
	}

	write.close();
}

bool UserDataConfig::ReadUserDataCfg()
{
	if( !ReadFromFile() )
	{
		CreateDefaultCfg();
	}

	if( !ReadFromFile() )
	{
		return false;
	}

	return true;
}


