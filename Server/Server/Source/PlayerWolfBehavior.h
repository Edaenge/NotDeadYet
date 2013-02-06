#include "Behavior.h"

class PlayerWolfBehavior : public PlayerBehavior
{
private:

public:
	PlayerWolfBehavior(Actor* actor, World* world);

	bool Update(float dt);
};