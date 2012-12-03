/*
	Made by Jensen Christopher Datum(03/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

enum KEY_BINDINGS
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPRINT,
	JUMP,
	DUCK,
	INTERACT,
	MENU
};
class Keybinds
{
public:
	Keybinds();
	virtual ~Keybinds();
	char GetKey(unsigned int key);
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