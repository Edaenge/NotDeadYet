/*
	Made by Simon Edänge Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <fstream>
#include "KeyValues.h"

/*This class reads the user bindings from KeyBindings.ini*/
class KeyBindConfig
{
public:
	KeyBindConfig();
	virtual ~KeyBindConfig();

	/*! Reads bindings from INI file.
		If not found, the function will call CreateDefaultIni.
	*/
	void ReadBindings(char* bindings);
	void CreateDefaultIni(char* bindings);
	bool WriteToFile(const std::string path, const char* data);
	bool ReadFromFile(const std::string path, char* data);
	/*! Converts a virtual char key to defined keys.
		These can be found in KeyConverter.*/
	bool ConvertKey(std::string& ret, const char key) const;
private:
	/*! Returns a key value used as an index in the key array.
		CC is an Key Press, e.g COMMAND_FORWARD
	*/
	int GetKeyValue(std::string KP);
	const std::string& GetCommand(const int keyValue);
	void TrimAndSet(char* ret);

};