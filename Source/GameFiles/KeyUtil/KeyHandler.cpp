#include "GameFiles/KeyUtil/KeyHandler.h"

KeyHandler::KeyHandler()
{
	this->zBinds = Keybinds();
	this->zStates = KeyStates();
}
KeyHandler::~KeyHandler()
{
}
char KeyHandler::GetKey(const unsigned int key)
{
	return this->zBinds.GetKey(key);
}
bool KeyHandler::GetKeyState(const unsigned int key)
{
	return this->zStates.GetKeyState(key);
}
void KeyHandler::SetKeyState(const unsigned int key, const bool value)
{
	this->zStates.SetKeyState(key, value);
}