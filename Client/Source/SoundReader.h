/*
Made by Ed�nge Simon Datum(01/02/13 created) 
for project Not Dead Yet at Blekinge tekniska h�gskola.
*/

#pragma once

#include "FileReader.h"
#include "SoundHandler.h"

class SoundReader : public FileReader
{
public:
	SoundReader(SoundHandler* engine);
	virtual ~SoundReader();

	virtual bool ReadFromFile();

private:
	SoundHandler* zEngine;
};
