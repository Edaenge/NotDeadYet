#include "Behavior.h"

class AIWolfBehavior : public Behavior
{
private:

public:
	AIWolfBehavior(Actor* actor, World* world);
	virtual ~AIWolfBehavior();

	bool Update(float dt);
};