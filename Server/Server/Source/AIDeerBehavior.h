#include "Behavior.h"

class AIDeerBehavior : public Behavior
{
private:

public:
	AIDeerBehavior(Actor* actor, World* world);

	bool Update(float dt);
};