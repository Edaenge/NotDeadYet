#pragma once

#include "Actor.h"


class BerryBushActor : public Actor
{
	bool zPicked;

public:
	BerryBushActor();
	virtual ~BerryBushActor();

	// Returns if the bush has been picked
	inline const bool& IsPicked() const { return zPicked; }

	// Set the picked state of the berry bush
	void SetPicked( bool pickedFlag );
};