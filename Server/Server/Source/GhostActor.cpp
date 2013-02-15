#include "GhostActor.h"

GhostActor::GhostActor(Player* player) : 
	zPlayer(player), Actor()
{
	this->zEnergy = 0;
}

GhostActor::~GhostActor()
{

}