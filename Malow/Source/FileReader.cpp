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
