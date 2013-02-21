//Created by Simon Edänge.

#pragma once

#include "FileReader.h"
#include <Vector>

class UserDataConfig : public FileReader
{
public:
	UserDataConfig();
	virtual ~UserDataConfig();

	bool ReadUserDataCfg();
	bool SaveUserDataCfg();

	const std::string& GetPlayerName() const {return this->zUserName;}
	const std::string& GetPlayerModel() const {return this->zUserModel;}

	void SetPlayerName(std::string name) {this->zUserName = name;}
	void SetPlayerModel(std::string model) {this->zUserModel = model;}
	virtual bool ReadFromFile();

private:
	void WriteToFile(std::vector<std::string>& data);
	void CreateDefaultCfg();
	bool InterpCommand(std::string command, std::string key);

private:

	std::string zUserName;
	std::string zUserModel;
};