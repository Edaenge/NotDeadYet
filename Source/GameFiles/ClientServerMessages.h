#pragma once

#include <string>
#include <fstream>
#include "../MaloWLib/MaloW.h"

static bool eraseFile = false;
static bool writeToFile = false;
namespace Messages
{
	inline void Debug(std::string msg)
	{
		if(eraseFile)
		{
			ofstream writeFile;
			writeFile.open ("ServerClientMessages.txt", ios::out | ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseFile = false;
		}


		fstream writeFile;
		writeFile.open ("ServerClientMessages.txt", fstream::in | fstream::out | fstream::app);

		writeFile << msg << endl;

		writeFile.close();
	}

	inline void Debug(float nr)
	{
		if(eraseFile)
		{
			ofstream writeFile;
			writeFile.open ("ServerClientMessages.txt", ios::out | ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseFile = false;
		}


		fstream writeFile;
		writeFile.open ("ServerClientMessages.txt", fstream::in | fstream::out | fstream::app);

		writeFile << MaloW::convertNrToString(nr) << endl;

		writeFile.close();
	}

	inline void ClearDebug()
	{
		//if(eraseFile)
		//{
		ofstream writeFile;
		writeFile.open ("ServerClientMessages.txt", ios::out | ios::trunc);

		writeFile << "";

		writeFile.close();
		eraseFile = false;
		//}
	}
	inline bool FileWrite()
	{
		return writeToFile;
	}
}