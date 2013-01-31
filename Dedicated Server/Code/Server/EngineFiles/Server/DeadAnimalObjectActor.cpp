#include "DeadAnimalObjectActor.h"

#define  MAX_DROP 3

DeadAnimalObjectActor::DeadAnimalObjectActor( const bool genID /*= false*/ ) : DeadActor(genID)
{
	this->zActorType = ACTOR_TYPE_DEAD_ANIMAL;

	srand((unsigned int)time(NULL));
}

DeadAnimalObjectActor::~DeadAnimalObjectActor()
{

}

void DeadAnimalObjectActor::GenerateDrop()
{
	int nrOfItem = (rand() % 1 + MAX_DROP) + 1;

	for (int i = 0; i < nrOfItem; i++)
	{
		
	}
}

