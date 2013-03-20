#pragma once

#include <World/World.h>

class ClientActorManager;
class Actor;
class iDecal;
class GraphicsEngine;
class World;


class FootStepClient : public Observer
{
	GraphicsEngine* zGraphics;
	ClientActorManager* zActorManager;
	World* zWorld;

	std::map<Actor*, Vector2> zLastPositions;
	std::map<Actor*, unsigned int> zLastFoot;

	std::set<Actor*> zActors;
	std::set<std::pair<unsigned int, iDecal*>> zFootSteps;
	unsigned int zCurrentDecal;

	// Purging
	Vector3 zLastPurgePos;

public:
	FootStepClient(GraphicsEngine* graphics, ClientActorManager* actorManager, World* zWorld);
	virtual ~FootStepClient();

	// Purge footsteps outside far clip
	void PurgeSteps();

	// Update Required Footsteps
	void Update();

	// Place Foot Step
	void PlaceFootStep(Actor* actor);

protected:
	void OnEvent(Event* e);
};