/*
	Made by Jensen Christopher Datum(03/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/KeyUtil/KeyValues.h"

class Keybinds
{
public:
	Keybinds();
	virtual ~Keybinds();
	char GetKey(const unsigned int key);
private:
	char zForwardKey;
	char zBackwardKey;
	char zLeftKey;
	char zRightKey;
	char zSprintKey;
	char zJumpKey;
	char zDuckKey;
	char zInteractKey;
	char zMenuKey;
};