#include "Physics.h"

namespace {
	static PhysicsEngine* engine = 0;
}

bool PhysicsInit()
{
	engine = new PhysicsEngine();
	return (engine != 0);
}

PhysicsEngine* GetPhysics()
{
	if ( !engine )
		throw("Engine Not Initialized!");

	return engine;
}

bool FreePhysics()
{
	if ( !engine )
		return 0;

	delete engine;

	return true;
}