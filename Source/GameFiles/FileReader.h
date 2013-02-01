/*
Made by Ed�nge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska h�gskola.
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
	virtual void TrimAndSet(char* ret);
	virtual bool Replace(char* key);


private:

};
