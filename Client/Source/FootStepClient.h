#pragma once

#include <World/World.h>

class ClientActorManager;
class Actor;
class iDecal;
class GraphicsEngine;


class FootStepClient : public Observer
{
	GraphicsEngine* zGraphics;
	ClientActorManager* zActorManager;
	std::map<Actor*, Vector2> zLastPositions;

	std::set<Actor*> zActors;
	std::set<iDecal*> zFootSteps;

public:
	FootStepClient(GraphicsEngine* graphics, ClientActorManager* actorManager);
	virtual ~FootStepClient();

	// Place Foot Step
	void PlaceFootStep(Actor* actor);

protected:
	void OnEvent(Event* e);
};