#include "GameFiles/KeyUtil/Keybinds.h"

#include "GameFiles/KeyUtil/KeyValues.h"

#include <windows.h>
Keybinds::Keybinds()
{
	this->zForwardKey = 'W';
	this->zBackwardKey = 'S';
	this->zLeftKey = 'A';
	this->zRightKey = 'D';
	this->zSprintKey = VK_SHIFT;
	this->zJumpKey = VK_SPACE;
	this->zDuckKey = VK_CONTROL;
	this->zInteractKey = 'E';
	this->zMenuKey = VK_ESCAPE;
}
Keybinds::~Keybinds()
{
}
char Keybinds::GetKey(const unsigned int key)
{
	char returnValue;
	switch (key)
	{
	case KEY_FORWARD:
		returnValue = this->zForwardKey;
		break;
	case KEY_BACKWARD:
		returnValue = this->zBackwardKey;
		break;
	case KEY_LEFT:
		returnValue = this->zLeftKey;
		break;
	case KEY_RIGHT:
		returnValue = this->zRightKey;
		break;
	case KEY_SPRINT:
		returnValue = this->zSprintKey;
		break;
	case KEY_JUMP:
		returnValue = this->zJumpKey;
		break;
	case KEY_DUCK:
		returnValue = this->zDuckKey;
		break;
	case KEY_INTERACT:
		returnValue = this->zInteractKey;
		break;
	case KEY_MENU:
		returnValue = this->zMenuKey;
		break;
	default:
		returnValue = ' ';
		break;
	}

	return returnValue;
}

