#include "AIBehavior.h"

class AIWolfBehavior : public AIBehavior
{
private:

public:
	AIWolfBehavior(Actor* actor, World* world);
	virtual ~AIWolfBehavior();

	bool Update(float dt);
};