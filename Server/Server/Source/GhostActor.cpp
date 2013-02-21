#include "GhostActor.h"

GhostActor::GhostActor(Player* player) : 
	zPlayer(player), Actor()
{
	this->zEnergy = 0;
	this->SetScale(Vector3(0.0f,0.0f,0.0f));
	this->zType = ACTOR_TYPE_GHOST;
}

GhostActor::~GhostActor()
{

}