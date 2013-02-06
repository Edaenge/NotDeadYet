#include "FileReader.h"


FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

void FileReader::TrimAndSet( std::string& str )
{
	if( str.empty() )
		return;

	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

	size_t found;
	found = str.find("=");

	if(found == std::string::npos)
		return;

	int index = (int)found;

	std::string subA;
	std::string subB;
	subA = str.substr(0,index);
	subB = str.substr(index+1,str.size()-1);

	str = subA +  " = " + subB;
}

void FileReader::TrimAndSet( const std::string& string, std::string& a, std::string& b )
{
	if( string.empty() )
		return;
	std::string str = string;
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

	size_t found;
	found = str.find("=");

	if(found == std::string::npos)
		return;

	int index = (int)found;

	a = str.substr(0,index);
	b = str.substr(index+1,str.size()-1);
}

bool FileReader::Replace( std::string& str )
{
	if (str.empty())
		return false;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '_')
		{
			str.at(i) = ' ';
		}
	}
	
	return true;
}
