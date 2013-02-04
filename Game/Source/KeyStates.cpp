#include "KeyStates.h"

KeyStates::KeyStates()
{
	for (unsigned int i = 0; i < KEY_CAP; i++)
	{
		this->zKeyState[i] = false;
	}
}
KeyStates::~KeyStates()
{
}
bool KeyStates::GetKeyState(const unsigned int key) const
{
	return this->zKeyState[key];
}
void KeyStates::SetKeyState(const unsigned int key, const bool value)
{
	this->zKeyState[key] = value;
}