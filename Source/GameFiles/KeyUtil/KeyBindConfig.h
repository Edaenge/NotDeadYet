/*
	Made by Simon Edänge Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <fstream>
#include "GameFiles/KeyUtil/KeyValues.h"


class KeyBindConfig
{
public:
	KeyBindConfig();
	~KeyBindConfig();

	void ReadBindings(char* bindings);
	void CreateDefaultIni(char* bindings);
	bool WriteToFile(const std::string path, const char* data);
	bool ReadFromFile(const std::string path, char* data);

private:
	/*! Returns a key value used as an index in the key array.
		KP is an Key Press, e.g KP_FORWARD
	*/
	int GetKeyValue(std::string KP);

};