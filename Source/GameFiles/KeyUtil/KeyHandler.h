#pragma once

#include "GameFiles/KeyUtil/Keybinds.h"
#include "GameFiles/KeyUtil/KeyStates.h"

class KeyHandler
{
private:
	Keybinds zBinds;
	KeyStates zStates;
public:
	KeyHandler();
	virtual ~KeyHandler();
	char GetKey(const unsigned int key);
	bool GetKeyState(const unsigned int key);
	void SetKeyState(const unsigned int key, const bool value);
};