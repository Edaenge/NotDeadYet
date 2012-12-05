/*
	Made by Jensen Christopher Datum(05/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once
#include "GameFiles/KeyUtil/KeyValues.h"

class KeyStates
{
public:
	KeyStates();
	virtual ~KeyStates();
	bool GetKeyState(const unsigned int key);
	void SetKeyState(const unsigned int key, const bool value);
private:
	bool zKeyState[KEY_CAP];
};