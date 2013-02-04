#include "Keybinds.h"
#include "KeyValues.h"
#include "KeyBindConfig.h"

#include <windows.h>
Keybinds::Keybinds()
{
	//this->zForwardKey = 'W';
	//this->zBackwardKey = 'S';
	//this->zLeftKey = 'A';
	//this->zRightKey = 'D';
	//this->zSprintKey = VK_SHIFT;
	//this->zJumpKey = VK_SPACE;
	//this->zDuckKey = VK_CONTROL;
	//this->zInteractKey = 'E';
	//this->zMenuKey = VK_ESCAPE;
}

Keybinds::~Keybinds()
{
}

char Keybinds::GetKey(const unsigned int key) const
{
	if(key < KEY_CAP)
		return this->zKeyBindings[key];

	return '\0';
}

void Keybinds::ReadKeyBinds()
{
	KeyBindConfig kbc;
	kbc.ReadBindings(this->zKeyBindings);
}