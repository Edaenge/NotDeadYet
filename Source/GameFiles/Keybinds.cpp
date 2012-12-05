#include "GameFiles/Keybinds.h"

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
	case FORWARD:
		returnValue = this->zForwardKey;
		break;
	case BACKWARD:
		returnValue = this->zBackwardKey;
		break;
	case LEFT:
		returnValue = this->zLeftKey;
		break;
	case RIGHT:
		returnValue = this->zRightKey;
		break;
	case SPRINT:
		returnValue = this->zSprintKey;
		break;
	case JUMP:
		returnValue = this->zJumpKey;
		break;
	case DUCK:
		returnValue = this->zDuckKey;
		break;
	case INTERACT:
		returnValue = this->zInteractKey;
		break;
	case MENU:
		returnValue = this->zMenuKey;
		break;
	default:
		returnValue = '-1';
		break;
	}

	return returnValue;
}

