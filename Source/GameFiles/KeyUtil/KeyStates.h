/*
	Made by Jensen Christopher Datum(05/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "KeyValues.h"

class KeyStates
{
public:
	KeyStates();
	virtual ~KeyStates();
	/*! Returns true if the key is pressed*/
	bool GetKeyState(const unsigned int key) const;
	/*! Change the value for the Key State*/
	void SetKeyState(const unsigned int key, const bool value);
private:
	bool zKeyState[KEY_CAP];
};