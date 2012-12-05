#include "GameFiles/KeyHandler.h"

KeyHandler::KeyHandler()
{
	this->zBinds = Keybinds();
	for (unsigned int i = 0; i < CAP; i++)
	{
		this->zKeyState[i] = false;
	}
}
KeyHandler::~KeyHandler()
{
}
bool KeyHandler::GetKeyState(const unsigned int key)
{
	return this->zKeyState[key];
}
void KeyHandler::SetKeyState(const unsigned int key, const bool value)
{
	this->zKeyState[key] = value;
}
char KeyHandler::GetKey(const unsigned int key)
{
	return this->zBinds.GetKey(key);
}
