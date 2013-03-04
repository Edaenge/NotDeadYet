#include "KeyStates.h"

KeyStates::KeyStates()
{
	for (unsigned int i = 0; i < KEY_CAP; i++)
		this->zKeyState[i] = false;
}
KeyStates::~KeyStates()
{
}
bool KeyStates::GetKeyState(const unsigned int key) const
{
	if (key < KEY_CAP)
		return this->zKeyState[key];

	return false;
}
void KeyStates::SetKeyState(const unsigned int key, const bool value)
{
	if (key < KEY_CAP)
		this->zKeyState[key] = value;
}
void KeyStates::ClearStates()
{
	for (unsigned int i = 0; i < KEY_CAP; i++)
		this->zKeyState[i] = false;
}