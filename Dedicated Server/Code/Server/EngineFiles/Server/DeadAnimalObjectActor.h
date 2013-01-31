/*
	Made by Simon Ed�nge Datum(31/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
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
