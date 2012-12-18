#pragma once

#include "PhysicsEngine.h"
#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib, "NDYGFXD.lib")
#else
#pragma comment(lib, "NDYGFX.lib")
#endif

bool PhysicsInit();
PhysicsEngine* GetPhysics();
bool FreePhysics();