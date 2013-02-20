#include "PlayerSettings.h"

namespace {
	static UserDataConfig* player_Settings = 0;
}

bool PlayerSettingsInit()
{
	player_Settings = new UserDataConfig();
	return player_Settings->ReadUserDataCfg();

}

UserDataConfig* GetPlayerSettings()
{
	if( !player_Settings )
		throw("Settings Not Initialized!");

	return player_Settings;
}

bool FreePlayerSettings()
{
	if(!player_Settings)
		return false;

	delete player_Settings, player_Settings = 0;

	return true;
}

bool SavePlayerSettings()
{
	if(!player_Settings)
		return false;

	player_Settings->SaveUserDataCfg();

	return true;
}