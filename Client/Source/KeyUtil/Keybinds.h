/*
	Made by Jensen Christopher Datum(03/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "KeyValues.h"

class Keybinds
{
public:
	Keybinds();
	virtual ~Keybinds();
	/*! Reads the key binds from .ini file.*/
	void ReadKeyBinds();
	/*! Returns the binding for the Key eg. 'w' or 's'*/
	char GetKey(const unsigned int key) const;
private:
	char zKeyBindings[KEY_CAP];
};