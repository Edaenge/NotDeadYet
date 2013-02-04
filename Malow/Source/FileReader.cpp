#include "FileReader.h"


FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

void FileReader::TrimAndSet( char* ret )
{
	if(ret == NULL)
		return;

	std::string str(ret);
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

	subA += " = ";
	str = subA + subB;

	// TODO: Not Correct
	strcpy(ret, str.c_str());
}

bool FileReader::Replace( char* key )
{
	if (strcmp(key, "") == 0)
		return false;

	std::string str = std::string(key);

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '_')
		{
			str.at(i) = ' ';
		}
	}

	// TODO: Not Correct
	strcpy(key, str.c_str());
	
	return true;
}
