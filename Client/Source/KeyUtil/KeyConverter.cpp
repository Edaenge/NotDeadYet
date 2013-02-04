#include "KeyConverter.h"
#include <windows.h>
#include <sstream>
//Common Keys
static const std::string K_SPACE = "SPACE";
static const std::string K_SHIFT = "SHIFT";
static const std::string K_ESCAPE = "ESCAPE";
static const std::string K_CONTROL = "CTRL";
static const std::string K_RETURN = "RETURN";
static const std::string K_LEFT = "LEFT";
static const std::string K_RIGHT = "RIGHT";
static const std::string K_UP = "UP";
static const std::string K_DOWN = "DOWN";
//Numpad Keys 0-9
static const std::string NP_0 = "NP_0";
static const std::string NP_1 = "NP_1";
static const std::string NP_2 = "NP_2";
static const std::string NP_3 = "NP_3";
static const std::string NP_4 = "NP_4";
static const std::string NP_5 = "NP_5";
static const std::string NP_6 = "NP_6";
static const std::string NP_7 = "NP_7";
static const std::string NP_8 = "NP_8";
static const std::string NP_9 = "NP_9";
//Numpad Keys * - /
static const std::string NP_MUL = "NP_MUL";
static const std::string NP_ADD = "NP_ADD";
static const std::string NP_SEP = "NP_SEP";
static const std::string NP_SUB = "NP_SUB";
static const std::string NP_DEC = "NP_DEC";
static const std::string NP_DIV = "NP_DIV";
//F Keys F1 - F12
static const std::string F1 = "F1";
static const std::string F2 = "F2";
static const std::string F3 = "F3";
static const std::string F4 = "F4";
static const std::string F5 = "F5";
static const std::string F6 = "F6";
static const std::string F7 = "F7";
static const std::string F8 = "F8";
static const std::string F9 = "F9";
static const std::string F10 = "F10";
static const std::string F11 = "F11";
static const std::string F12 = "F12";
//Uncommon Keys
static const std::string U_PRINT = "PRINT";
static const std::string U_INSERT = "INSERT";
static const std::string U_DELETE = "DELETE";
static const std::string U_HOME = "HOME";
static const std::string U_PAUSE = "PAUSE";
static const std::string U_END = "END";
static const std::string U_CAPITAL = "CAPITAL";
static const std::string U_MENU = "MENU";
static const std::string U_HELP = "HELP";
static const std::string U_SLEEP = "SLEEP";
static const std::string U_SNAPSHOT = "SNAPSHOT";
static const std::string U_SELECT = "SELECT";

KeyConverter::KeyConverter()
{
}

KeyConverter::~KeyConverter()
{
}

bool KeyConverter::CheckNumPad(std::string& ret, const char key) const
{
	bool found = true;
	switch (key)
	{
	case VK_NUMPAD0:
		ret = NP_0;
		break;
	case VK_NUMPAD1:
		ret = NP_1;
		break;
	case VK_NUMPAD2:
		ret = NP_2;
		break;
	case VK_NUMPAD3:
		ret = NP_3;
		break;
	case VK_NUMPAD4:
		ret = NP_4;
		break;
	case VK_NUMPAD5:
		ret = NP_5;
		break;
	case VK_NUMPAD6:
		ret = NP_6;
		break;
	case VK_NUMPAD7:
		ret = NP_7;
		break;
	case VK_NUMPAD8:
		ret = NP_8;
		break;
	case VK_NUMPAD9:
		ret = NP_9;
		break;
	case VK_MULTIPLY:
		ret = NP_MUL;
		break;
	case VK_ADD:
		ret = NP_ADD;
		break;
	case VK_SEPARATOR:
		ret = NP_SEP;
		break;
	case VK_SUBTRACT:
		ret = NP_SUB;
		break;
	case VK_DECIMAL:
		ret = NP_DEC;
		break;
	case VK_DIVIDE:
		ret = NP_DIV;
		break;
	default:
		found = false;
		break;
	}
	return found;
}

bool KeyConverter::CheckFKeys(std::string& ret, const char key ) const
{
	bool found = true;
	switch (key)
	{
	case VK_F1:
		ret = F1;
		break;
	case VK_F2:
		ret = F2;
		break;
	case VK_F3:
		ret = F3;
		break;
	case VK_F4:
		ret = F4;
		break;
	case VK_F5:
		ret = F5;
		break;
	case VK_F6:
		ret = F6;
		break;
	case VK_F7:
		ret = F7;
		break;
	case VK_F8:
		ret = F8;
		break;
	case VK_F9:
		ret = F9;
		break;
	case VK_F10:
		ret = F10;
		break;
	case VK_F11:
		ret = F11;
		break;
	case VK_F12:
		ret = F12;
		break;
	default:
		found = false;
		break;
	}
	return found;

}

bool KeyConverter::CheckCommonKeys(std::string& ret, const char key) const
{
	bool found = true;
	switch (key)
	{
	case VK_SHIFT:
		ret = K_SHIFT;
		break;
	case VK_ESCAPE:
		ret = K_ESCAPE;
		break;
	case VK_SPACE:
		ret = K_SPACE;
		break;
	case VK_RETURN:
		ret = K_RETURN;
		break;
	case VK_CONTROL:
		ret = K_CONTROL;
		break;
	case VK_LEFT:
		ret = K_LEFT;
		break;
	case VK_UP:
		ret = K_UP;
		break;
	case VK_RIGHT:
		ret = K_RIGHT;
		break;
	case VK_DOWN:
		ret = K_DOWN;
		break;
	default:
		found = false;
		break;
	}
	return found;
}

bool KeyConverter::CheckUncommonKeys(std::string& ret, const char key) const
{
	bool found = true;
	switch (key)
	{
	case VK_PRINT:
		ret = U_PRINT;
		break;
	case VK_INSERT:
		ret = U_INSERT;
		break;
	case VK_DELETE:
		ret = U_DELETE;
		break;
	case VK_PAUSE:
		ret = U_PAUSE;
		break;
	case VK_HOME:
		ret = U_HOME;
		break;
	case VK_HELP:
		ret = U_HELP;
		break;
	case VK_CAPITAL:
		ret = U_CAPITAL;
		break;
	case VK_END:
		ret = U_END;
		break;
	case VK_SELECT:
		ret = U_SELECT;
		break;
	case VK_MENU:
		ret = U_MENU;
		break;
	case VK_SLEEP:
		ret = U_SLEEP;
		break;
	case VK_SNAPSHOT:
		ret = U_SNAPSHOT;
		break;
	default:
		found = false;
		break;
	}
	return found;
}

char KeyConverter::ConvertToChar(const std::string& key) const
{
	char returnValue = '\0';
	//Common Keys
	if(key == K_ESCAPE)
		returnValue = VK_ESCAPE;
	else if(key == K_SHIFT)
		returnValue = VK_SHIFT;
	else if(key == K_CONTROL)
		returnValue = VK_CONTROL;
	else if(key == K_RETURN)
		returnValue = VK_RETURN;
	else if(key == K_SPACE)
		returnValue = VK_SPACE;
	else if(key == K_LEFT)
		returnValue = VK_LEFT;
	else if(key == K_RIGHT)
		returnValue = VK_RIGHT;
	else if(key == K_DOWN)
		returnValue = VK_DOWN;
	else if(key == K_UP)
		returnValue = VK_UP;
	//Numpad
	else if(key == NP_0)
		returnValue = VK_NUMPAD0;
	else if(key == NP_1)
		returnValue = VK_NUMPAD1;
	else if(key == NP_2)
		returnValue = VK_NUMPAD2;
	else if(key == NP_3)
		returnValue = VK_NUMPAD3;
	else if(key == NP_4)
		returnValue = VK_NUMPAD4;
	else if(key == NP_5)
		returnValue = VK_NUMPAD5;
	else if(key == NP_6)
		returnValue = VK_NUMPAD6;
	else if(key == NP_7)
		returnValue = VK_NUMPAD7;
	else if(key == NP_8)
		returnValue = VK_NUMPAD8;
	else if(key == NP_9)
		returnValue = VK_NUMPAD9;
	else if(key == NP_MUL)
		returnValue = VK_MULTIPLY;
	else if(key == NP_ADD)
		returnValue = VK_ADD;
	else if(key == NP_SEP)
		returnValue = VK_SEPARATOR;
	else if(key == NP_SUB)
		returnValue = VK_SUBTRACT;
	else if(key == NP_DEC)
		returnValue = VK_DECIMAL;
	else if(key == NP_DIV)
		returnValue = VK_DIVIDE;
	//F Keys
	else if(key == F1)
		returnValue = VK_F1;
	else if(key == F2)
		returnValue = VK_F2;
	else if(key == F3)
		returnValue = VK_F3;
	else if(key == F4)
		returnValue = VK_F4;
	else if(key == F5)
		returnValue = VK_F5;
	else if(key == F6)
		returnValue = VK_F6;
	else if(key == F7)
		returnValue = VK_F7;
	else if(key == F8)
		returnValue = VK_F8;
	else if(key == F9)
		returnValue = VK_F9;
	else if(key == F10)
		returnValue = VK_F10;
	else if(key == F11)
		returnValue = VK_F11;
	else if(key == F12)
		returnValue = VK_F12;
	//Uncommon Keys
	else if(key == U_PRINT)
		returnValue = VK_PRINT;
	else if(key == U_INSERT)
		returnValue = VK_INSERT;
	else if(key == U_DELETE)
		returnValue = VK_DELETE;
	else if(key == U_HOME)
		returnValue = VK_HOME;
	else if(key == U_PAUSE)
		returnValue = VK_PAUSE;
	else if(key == U_END)
		returnValue = VK_END;
	else if(key == U_CAPITAL)
		returnValue = VK_CAPITAL;
	else if(key == U_MENU)
		returnValue = VK_MENU;
	else if(key == U_HELP)
		returnValue = VK_HELP;
	else if(key == U_SLEEP)
		returnValue = VK_SLEEP;
	else if(key == U_SNAPSHOT)
		returnValue = VK_SNAPSHOT;
	else if(key == U_SELECT)
		returnValue = VK_SELECT;

	return returnValue;
}
