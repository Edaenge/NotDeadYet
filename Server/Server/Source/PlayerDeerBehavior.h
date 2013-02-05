#include "Behavior.h"

class PlayerDeerBehavior : public Behavior
{
private:

public:
	PlayerDeerBehavior(Actor* actor, World* world);

	bool Update(float dt);
};