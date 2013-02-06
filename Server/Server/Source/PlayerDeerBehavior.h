#include "PlayerBehavior.h"

class PlayerDeerBehavior : public PlayerBehavior
{
private:

public:
	PlayerDeerBehavior(Actor* actor, World* world, Player* palyer);
	virtual ~PlayerDeerBehavior();

	bool Update(float dt);
};