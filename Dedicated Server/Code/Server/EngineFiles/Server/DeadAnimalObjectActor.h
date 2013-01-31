/*
	Made by Simon Edänge Datum(31/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "DeadActor.h"
#include "Food.h"
#include <ctime>

class DeadAnimalObjectActor : public DeadActor
{
public:
	DeadAnimalObjectActor(const bool genID = false);
	virtual ~DeadAnimalObjectActor();

	void GenerateDrop();

private:

};
