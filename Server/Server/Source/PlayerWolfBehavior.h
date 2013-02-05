#include "Behavior.h"

class PlayerWolfBehavior : public Behavior
{
private:

public:
	PlayerWolfBehavior(Actor* actor, World* world);

	bool Update(float dt);
};