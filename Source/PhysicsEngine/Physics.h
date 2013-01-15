#pragma once

#include "PhysicsEngine.h"
#include <windows.h>

bool PhysicsInit();
PhysicsEngine* GetPhysics();
bool FreePhysics();