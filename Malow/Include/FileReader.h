/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include <fstream>
#include <algorithm>
#include <string>

class FileReader
{
public:
	FileReader();
	virtual ~FileReader();

	virtual bool ReadFromFile() = 0;

protected:
	virtual void TrimAndSet(std::string& string);
	virtual void TrimAndSet(const std::string& string, std::string& a, std::string& b);
	virtual bool Replace(std::string& string);


private:

};
