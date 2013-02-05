#pragma once
class Actor // For testing
{

};

class World // For testing
{

};


class Behavior
{
private:
	Actor* zActor;
	World* zWorld;
public:
	Behavior(Actor* actor, World* world);
	virtual ~Behavior();

	virtual bool Update(float dt);

};