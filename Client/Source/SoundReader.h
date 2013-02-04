/*
Made by Edänge Simon Datum(01/02/13 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
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
