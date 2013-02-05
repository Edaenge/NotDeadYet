#include "Behavior.h"

class AIWolfBehavior : public Behavior
{
private:

public:
	AIWolfBehavior(Actor* actor, World* world);

	bool Update(float dt);
};