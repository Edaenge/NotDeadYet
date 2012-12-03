#include "GameFiles/KeyHandler.h"

KeyHandler::KeyHandler()
{
	this->zBinds = Keybinds();
}
KeyHandler::~KeyHandler()
{
}
bool KeyHandler::GetKeyState(unsigned int key)
{
	bool returnValue = false;
	switch (key)
	{
	case FORWARD:
		returnValue = this->zForwardPressed;
		break;
	case BACKWARD:
		returnValue = this->zBackwardPressed;
		break;
	case LEFT:
		returnValue = this->zLeftPressed;
		break;
	case RIGHT:
		returnValue = this->zRightPressed;
		break;
	case SPRINT:
		returnValue = this->zSprintPressed;
		break;
	case JUMP:
		returnValue = this->zJumpPressed;
		break;
	case DUCK:
		returnValue = this->zDuckPressed;
		break;
	case INTERACT:
		returnValue = this->zInteractPressed;
		break;
	case MENU:
		returnValue = this->zMenuPressed;
		break;
	default:
		returnValue = false;
		break;
	}

	return returnValue;
}
void KeyHandler::SetKeyState(unsigned int key, bool value)
{
	switch (key)
	{
	case FORWARD:
		this->zForwardPressed = value;
		break;
	case BACKWARD:
		this->zBackwardPressed = value;
		break;
	case LEFT:
		this->zLeftPressed = value;
		break;
	case RIGHT:
		this->zRightPressed = value;
		break;
	case SPRINT:
		this->zSprintPressed = value;
		break;
	case JUMP:
		this->zJumpPressed = value;
		break;
	case DUCK:
		this->zDuckPressed = value;
		break;
	case INTERACT:
		this->zInteractPressed = value;
		break;
	case MENU:
		this->zMenuPressed = value;
		break;
	default:
		break;
	}
}
char KeyHandler::GetKey(unsigned int key)
{
	return this->zBinds.GetKey(key);
}
