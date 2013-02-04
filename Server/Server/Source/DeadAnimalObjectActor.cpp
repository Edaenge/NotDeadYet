#include "DeadAnimalObjectActor.h"

#define  MAX_DROP 3

DeadAnimalObjectActor::DeadAnimalObjectActor( const bool genID /*= false*/ ) : DeadActor(genID)
{
	this->zActorType = ACTOR_TYPE_DEAD_ANIMAL;

	srand((unsigned int)time(NULL));
}

DeadAnimalObjectActor::~DeadAnimalObjectActor()
{
	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		SAFE_DELETE(*it);
	}
}

bool DeadAnimalObjectActor::GenerateDrop(Item* food)
{
	if(!food)
		return false;

	Food* fo = dynamic_cast<Food*>(food);

	if(!fo)
		return false;

	int nrOfItems = (rand() % 1 + MAX_DROP) + 1;

	for (int i = 0; i < nrOfItems; i++)
	{
		Food* new_food = new Food(fo);
		new_food->SetItemID(GenerateID());

		this->zItems.push_back(new_food);
	}

	return true;
}