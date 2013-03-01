#include "PlayerConfigReader.h"
#include "MaloW.h"

static const std::string FILENAME = "playerConditionVariables.txt";

PlayerConfigReader::PlayerConfigReader()
{

}

PlayerConfigReader::~PlayerConfigReader()
{

}

bool PlayerConfigReader::ReadFromFile()
{
	std::ifstream read;
	read.open(FILENAME);

	if (!read.is_open())
		return false;

	while (!read.eof())
	{
		std::string line;
		std::string key;
		std::string value;

		std::getline(read, line);
		
		if (line.empty())
			continue;

		TrimAndSet(line, key, value);

		this->zVariables[key] = MaloW::convertStringToFloat(value);
	}
}

float PlayerConfigReader::GetVariable( std::string mappedName )
{
	auto it = this->zVariables.find(mappedName);

	if (it != this->zVariables.end())
		return it->second;

	return 1.0f;
}

PlayerConfigReader* GetPlayerConfig()
{

}

void InitPlayerConfig()
{

}

void FreePlayerConfig()
{

}
