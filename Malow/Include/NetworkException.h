#pragma once

#include <string>

class NetworkException
{
public:
	std::string errString;
	int errCode;

	NetworkException( const std::string& string, unsigned int winsockError ) :
		errString(string),
		errCode(winsockError)
	{
	}
};