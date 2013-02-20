#pragma once

#include "UserDataConfig.h"

bool PlayerSettingsInit();
UserDataConfig* GetPlayerSettings();
bool FreePlayerSettings();
bool SavePlayerSettings();