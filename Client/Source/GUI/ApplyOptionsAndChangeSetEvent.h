#pragma once
/**
* Author: Rikard Johansson
* Create Date: 04/04/2012
* This is a event that returns that nothing is going to happen.
**/
#include "GUIEvent.h"

class ApplyOptionsAndChangeSetEvent : public GUIEvent
{
private:
	int set;
public:
	ApplyOptionsAndChangeSetEvent(int set);
	virtual ~ApplyOptionsAndChangeSetEvent();

	int GetSet(){ return this->set; }
};