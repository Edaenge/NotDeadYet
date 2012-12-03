#pragma once

#include "Keybinds.h"

class KeyHandler
{
private:
	bool zForwardPressed;
	bool zBackwardPressed;
	bool zLeftPressed;
	bool zRightPressed;
	bool zJumpPressed;
	bool zSprintPressed;
	bool zDuckPressed;
	bool zInteractPressed;
	bool zMenuPressed;
	Keybinds zBinds;
public:
	KeyHandler();
	~KeyHandler();
	bool GetKeyState(const unsigned int key);
	char GetKey(const unsigned int key);
	void SetKeyState(const unsigned int key, bool value);
};