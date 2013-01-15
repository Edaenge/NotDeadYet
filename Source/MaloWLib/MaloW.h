#ifndef MALOW_H
#define MALOW_H

#include <string>
#include <sstream>
#include <Windows.h>

#define MEMLEAKTEST _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

namespace MaloW
{
	inline void convertToCharArr(char arr[], std::string msg)
	{
		for(int i = 0; i < (int)msg.length(); i++)
			arr[i] = msg[i];
	}

	inline void convertFromCharArr(char arr[], std::string& msg)
	{
		msg = "";
		for(int i = 0; i < arr[i] != 0 && i < 512; i++)
			msg += arr[i];
	}
	
	inline std::string convertNrToString(float nr)
	{
		std::string s;
		std::stringstream out;
		out << nr;
		s = out.str();
		return s;
	}

	inline int convertStringToInt(const std::string& str)
	{
		int ret;
		std::stringstream ss;
		ss << str;
		ss >> ret;

		return ret;
	}

	inline float convertStringToFloat(const std::string& str)
	{
		float ret;
		std::stringstream ss;
		ss << str;
		ss >> ret;

		return ret;
	}

	inline std::wstring convertStringToWString(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
}
#endif